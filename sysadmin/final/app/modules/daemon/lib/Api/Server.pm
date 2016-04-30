package Api::Server;
use Api::Common;

use IO::Socket::INET;
use JSON;
use Switch;
use Try::Tiny;
use Linux::usermod;

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

  my $data = "";
  $client_socket->recv($data, 1024);

  print "Received: $data";
  my $error = 0;

  if ($self->{hard_fail}) {
    do_command($client_socket, $data);
  } else {
    try {
      do_command($client_socket, $data);
    } catch {
      print "JSON error caught: $data";
      $error = 1;
    };
  }

  if ($error) {
    $client_socket->send(encode_json({ error => JSON::true }));
  }

  # notify client that response has been sent
  shutdown($client_socket, 1);
}

# "Class methods"
sub do_command {
  my ($client, $command) = @_;

  $command = decode_json($command);

  my $reencoded = encode_json($command);
  print "command: $reencoded";

  my $result = 0;
  switch ($command->{command}) {
    case "login" {
      $result = check_user_login($command->{username},
                                 $command->{password});
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
    case "delete_user" {
      $result = delete_user($command->{username})
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

sub check_user_login {
  my ($username, $password) = @_;

  if (!user_exists($username)) {
    return 0;
  }

  my $user = Linux::usermod->new($username);

  # TODO: Use PAM or something like that?
  my $encrypted_password = $user->get('password');
  my $hash = crypt($password, $encrypted_password);

  # Woohoo!
  return $hash eq $encrypted_password;
}

sub group_exists {
  my $group = shift;
  my %groups = Linux::usermod->grps();

  return exists $groups{$group};
}

sub delete_user {
  my $username = shift;

  return 0 unless user_exists($username);

  Linux::usermod->del($username);
  return 1;
}

sub create_user {
  my ($username, $password, $type) = @_;

  if ($type ne "alumn" and $type ne "teacher") {
    return 0;
  }

  if (user_exists($username)) {
    return 0;
  }

  Linux::usermod->add($username, $password);

  my $user = Linux::usermod->new($username);
  $user->set("shell", "/bin/false");
  $user->set("home", "/home/" . $username);

  my $groupname = $type . "s";
  if (!group_exists($groupname)) {
    Linux::usermod->grpadd($groupname);
  }

  my $group = Linux::usermod->new($groupname, 1);
  my @group_users = $group->get("users");
  push @group_users, $username;

  my $gid = $group->get("gid");
  $group->set("users", "@users");

  return 1;
}

1;
