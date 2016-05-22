package Api::Server;
use Api::Common;

use IO::Socket::INET;
use JSON;
use Switch;
use Try::Tiny;
use Linux::usermod;
use File::Copy::Recursive qw(dircopy);
use File::Path qw(make_path remove_tree);
use File::Finder;

my @ALLOWED_GROUPS = ("teachers", "alumns");

# Creates the application server, this should be done just
# by the daemon
sub new {
  my $class = shift;
  my $self = {};
  bless $self, $class;

  $self->{handle} = new IO::Socket::INET (
    LocalHost => '127.0.0.1',
    LocalPort => Api::Common->port(),
    Proto => 'tcp',
    Listen => 5,
    Reuse => 1
  ) or die("couldn't create socket");

  $self->{hard_fail} = 0;

  return $self;
}

sub close {
  my $self = shift;
  $self->{handle}->close();
}

sub hard_fail {
  my $self = shift;
  $self->{hard_fail} = 1;
}

sub wait_and_dispatch_command {
  my $self = shift;

  my $client_socket = $self->{handle}->accept();

  my $client_address = $client_socket->peerhost();
  my $client_port = $client_socket->peerport();
  print "connection from $client_address:$client_port\n";

  my $data = <$client_socket>;

  my $error = 0;
  if ($self->{hard_fail}) {
    do_command($client_socket, $data);
  } else {
    try {
      do_command($client_socket, $data);
    } catch {
      print "JSON error caught: $data\n";
      $error = 1;
    };
  }

  if ($error) {
    $client_socket->send(encode_json({ error => JSON::true }));
  }

  # notify client that response has been sent
  shutdown($client_socket, 1);
}

sub update_user_password {
  my ($username, $password) = @_;

  return 0 unless $username;
  return 0 unless $password;
  return 0 unless user_exists($username);

  $user = Linux::usermod->new($username);
  $user->set('password', $password);

  return 1;
}

# "Class methods"
sub do_command {
  my ($client, $command) = @_;

  $command = decode_json($command);

  my $result = 0;

  print "Command: $command->{command}\n";

  switch ($command->{command}) {
    case "login" {
      my ($ok, $token, @groups) = check_user_login($command->{username},
                                                   $command->{password});

      if ($ok) {
        $client->send(encode_json({
          result => JSON::true,
          token => $token,
          groups => \@groups
        }));
        return;
      }
    }
    case "user_in_group" {
      $result = user_in_group($command->{username},
                              $command->{groupname});
    }
    case "check_login_token" {
      $result = check_login_token($command->{username},
                                  $command->{token});
    }
    case "user_exists" {
      $result = user_exists($command->{username});
    }
    case "group_exists" {
      $result = group_exists($command->{groupname});
    }
    case "create_user" {
      $result = create_user($command->{username},
                            $command->{password},
                            $command->{type});
    }
    case "update_user_password" {
      $result = update_user_password($command->{username},
                                     $command->{password});
    }
    case "delete_user" {
      $result = delete_user($command->{username})
    }
    case "set_feature" {
      $result = set_feature($command->{username},
                            $command->{feature},
                            $command->{value});
    }
  }

  if ($result) {
    $client->send(encode_json({ result => JSON::true }));
  } else {
    $client->send(encode_json({ result => JSON::false }));
  }
}

sub user_exists {
  my $username = shift;
  my %users = Linux::usermod->users();

  return exists $users{$username};
}

sub user_in_group {
  my ($username, $groupname) = @_;

  return 0 unless user_exists($username);
  return 0 unless group_exists($groupname);

  my $group = Linux::usermod->new($groupname, 1);
  my @users = split(",", $group->get("users"));

  return grep {$username eq $_} @users;
}

sub check_user_login {
  my ($username, $password) = @_;

  return 0 unless user_exists($username);

  # Only allow logging in users from allowed groups
  my @allowed_groups = ();
  foreach $allowed_group (@ALLOWED_GROUPS) {
    if (user_in_group($username, $allowed_group)) {
      push @allowed_groups, $allowed_group;
    }
  }

  return 0 unless scalar @allowed_groups;

  print "Groups: @allowed_groups " . scalar @allowed_groups . "\n";

  my $user = Linux::usermod->new($username);

  my $encrypted_password = $user->get('password');
  my $hash = crypt($password, $encrypted_password);

  # Woohoo!
  if ($hash eq $encrypted_password) {
    return (1, $encrypted_password, @allowed_groups);
  }

  return 0;
}

sub check_login_token {
  my ($username, $token) = @_;

  return 0 unless user_exists($username);

  my $user = Linux::usermod->new($username);
  my $encrypted_password = $user->get('password');

  return $encrypted_password eq $token;
}

sub group_exists {
  my $group = shift;
  my %groups = Linux::usermod->grps();

  return exists $groups{$group};
}

sub delete_user {
  my $username = shift;

  return 0 unless user_exists($username);
  return 0 if $username eq "root";

  # Remove from any group before removing the user
  foreach $allowed_group (@ALLOWED_GROUPS) {
    if (user_in_group($username, $allowed_group)) {
      my $group = Linux::usermod->new($allowed_group, 1);
      my @group_users = split(",", $group->get("users"));
      @group_users = grep { $_ ne $username } @group_users;
      $group->set("users", join(" ", @group_users));
    }
  }

  Linux::usermod->del($username);
  Linux::usermod->grpdel($username) if group_exists($username);
  remove_tree("/home/$username");

  return 1;
}

sub create_user {
  my ($username, $password, $type) = @_;

  my $allowed_type = 0;
  foreach $allowed_group (@ALLOWED_GROUPS) {
    if ($type . "s" eq $allowed_group) {
      $allowed_type = 1;
      last;
    }
  }

  return 0 unless $allowed_type;

  return 0 if user_exists($username);

  Linux::usermod->add($username, $password);
  Linux::usermod->grpadd($username);

  my $user = Linux::usermod->new($username);
  my $usergrp = Linux::usermod->new($username, 1);

  # Mark as primary group
  $user->set('gid', $usergrp->get('gid'));

  # Setup home directory
  my $homedir = "/home/$username";
  make_path($homedir, { owner => $username, group => $username });
  dircopy("/etc/skel", $homedir);
  chown($user->get('uid'), $usergrp->get('gid'), File::Finder->in($homedir));

  $user->set("shell", "/bin/false");
  $user->set("home", $homedir);

  my $groupname = $type . "s";
  if (!group_exists($groupname)) {
    Linux::usermod->grpadd($groupname);
    create_shared_folder() if $type eq "teacher";
  }

  my $group = Linux::usermod->new($groupname, 1);

  my @group_users = split(",", $group->get("users"));
  push @group_users, $username;
  @group_users = grep { $_ ne '' } @group_users;

  $group->set("users", join(" ", @group_users));

  return 1;
}

# This creates the shared folder once a teacher has been created
# TODO: Don't hardcode shit here
sub create_shared_folder {
  die("teachers group should exist") unless group_exists("teachers");
  my $shared_dir = "/etc/sysadmin-app/apuntes";
  remove_tree($shared_dir);
  make_path($shared_dir, { owner => "root", group => "teachers" });
  chmod(0775, $shared_dir);
}

sub set_feature {
  my ($username, $feature, $value) = @_;

  print "set_feature: $username, $feature, $value\n";

  return 0 unless user_exists($username);

  switch ($feature) {
    case "personal_page" {
      if ($value) {
        return setup_personal_webpage($username);
      } else {
        return remove_personal_page($username);
      }
    }
  }

  return 0;
}

sub set_owner {
  my ($username, $folder) = @_;
  my $user = Linux::usermod->new($username);
  my $usergrp = Linux::usermod->new($username, 1);
  chown($user->get('uid'), $usergrp->get('gid'), File::Finder->in($folder));
}

sub remove_personal_page {
  if (-d "/home/$username/public_html") {
    move("/home/$username/public_html", "/home/$username/public_html.bak");
    set_owner($username, "/home/$username/public_html");
  }

  return 1;
}

sub setup_personal_webpage {
  my $username = shift;

  print "Setting personal webpage for $username\n";

  if (-d "/home/$username/public_html") {
    return 1;
  }

  if (-d "/home/$username/public_html.bak") {
    move("/home/$username/public_html.bak", "/home/$username/public_html");
    set_owner($username, "/home/$username/public_html");
    return 1;
  }

  make_path("/home/$username/public_html", { owner => $username, group => $username });
  dircopy("/etc/skel/public_html", "/home/$username/public_html");
  set_owner($username, "/home/$username/public_html");
  return 1;
}

1;
