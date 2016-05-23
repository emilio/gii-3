package Api::Client;
use Email::Valid;
use Config::General;
use Api::Common;
use JSON;
use Carp;
use DBI;
use Session::Token;
use Email::Send::SMTP::Gmail;

use IO::Socket::INET;

my $CONFIG_HANDLE = Config::General->new("/etc/sysadmin-app/sysadminapprc");
my %CONFIG = $CONFIG_HANDLE->getall();

my $DB = DBI->connect("dbi:Pg:dbname=$CONFIG{'db_name'}",
                      $CONFIG{'db_user'},
                      $CONFIG{'db_pass'});

croak "Invalid DB handle: $CONFIG{'db_user'}, $CONFIG{'db_name'}" unless $DB->ping();


sub get_mailer {
  return Email::Send::SMTP::Gmail->new(
    -smtp => 'smtp.gmail.com',
    -login => $CONFIG{'gmail_mailer_login'},
    -port => 587,
    -pass => $CONFIG{'gmail_mailer_pass'});
}

# Api consumer, for now one needs to be created per API call,
# which is kind of expensive.
sub new {
  my $class = shift;
  my $self = {};
  bless $self, $class;

  return $self;
}

sub handle {
  my $self = shift;

  # TODO: When we're able to reuse the
  # connection, do it here instead of
  # creating a new socket each time.
  my $socket = new IO::Socket::INET (
    PeerHost => '127.0.0.1',
    PeerPort => Api::Common->port(),
    Proto => 'tcp',
  ) or die("couldn't create socket");

  autoflush $socket 1;

  return $socket;
}

sub call {
  my ($self, %command) = @_;

  my $encoded = encode_json(\%command);

  my $socket = $self->handle();
  $socket->send($encoded);

  # Notify we've stopped writing.
  shutdown($socket, 1);

  my $data = <$socket>;
  my $response = decode_json($data);

  return $response;
}

sub user_exists {
  my ($self, $username) = @_;

  my $response = $self->call(
      command => "user_exists",
      username => $username,
  );

  return $response->{result} eq JSON::true;
}

sub group_exists {
  my ($self, $groupname) = @_;

  my $response = $self->call(
      command => "group_exists",
      groupname => $groupname,
  );

  return $response->{result} eq JSON::true;
}

sub delete_user {
  my ($self, $username) = @_;

  my $statement = $DB->prepare("DELETE FROM users WHERE username = ?");

  # Consciuously ignore the error
  $statement->execute($username);

  my $response = $self->call(
      command => "delete_user",
      username => $username
  );

  return $response->{result} eq JSON::true;
}

# NOTE: This also updates the token to null if not present as parameter.
sub update_user_data {
  my ($self, $username, $email, $address, $token) = @_;

  # We do an insert and do nothing on failure, then an update.
  my $statement = $DB->prepare("INSERT INTO users (username, email, address, token) \
                               VALUES (?, ?, ?, ?)");
  return 1 if $statement->execute($username, $email, $address, $token);

  $statement = $DB->prepare("UPDATE users SET email = ?, address = ?, token = ? WHERE \
                             username = ?");

  return 1 if $statement->execute($email, $address, $token, $username);

  return 0;
}

sub get_user_data {
  my ($self, $username) = @_;

  # We do an insert and do nothing on failure, then an update.
  my $statement = $DB->prepare("SELECT username, email, address, token FROM users \
                                WHERE username = ?");
  return undef unless $statement->execute($username);

  my ($_username, $email, $address, $token) = $statement->fetchrow_array();

  my %ret = ();

  $ret{'email'} = $email;
  $ret{'address'} = $address;
  $ret{'token'} = $token;

  return %ret;
}

# TODO: Also receive $email and $address, and either give them to the server, or
# create the database record here.
sub create_user {
  my ($self, $username, $password, $type, $email, $address) = @_;

  if ($email and $address) {
    return 0 unless Email::Valid->address($email);
    return 0 unless $self->update_user_data($username, $email, $address);
  }

  my $response = $self->call(
      command => "create_user",
      username => $username,
      password => $password,
      type => $type,
  );

  return $response->{result} eq JSON::true;
}

sub user_in_group {
  my ($self, $username, $group) = @_;

  my $response = $self->call(
      command => "user_in_group",
      username => $username,
      groupname => $group
  );
}

sub generate_remember_password_token_and_send_email {
  my ($self, $username, $remind_url) = @_;

  # Check that we have the user data and that the user exists
  my $statement = $DB->prepare("SELECT username, email FROM users WHERE username = ?");
  return 0 unless $statement->execute($username);

  my ($_username, $email) = $statement->fetchrow_array();
  my $token = Session::Token->new->get;

  $statement = $DB->prepare("UPDATE users SET token = ? WHERE username = ?");
  return 0 unless $statement->execute($token, $username);

  return $self->send_password_reminder_to_user($username, $email, $token, $remind_url);
}

sub send_password_reminder_to_user {
  my ($self, $username, $email, $token, $remind_url) = @_;

  my $url = "$remind_url?username=$username\&token=$token";

  $self->send_generic_email($email,
                            "[sysadmin-app] Password reminder for $username",
                            "Please go to $url if this was intended.");

  return 1;
}

sub send_generic_email {
  my ($self, $email, $subject, $body, $attachments) = @_;

  my %params = (
    -to => $email,
    -subject => $subject,
    -body => $body,
  );

  if (defined $attachments) {
    $params{'-attachments'} = $attachments;
  }

  my $mailer = get_mailer();

  $mailer->send(%params);

  $mailer->bye;
}

sub check_login {
  my ($self, $username, $password) = @_;

  my $response = $self->call(
      command => "login",
      username => $username,
      password => $password
  );

  if ($response->{result} ne JSON::true) {
    return (0, undef, undef);
  }

  return (1, $response->{token}, $response->{groups});
}

sub check_login_token {
  my ($self, $username, $token) = @_;

  my $response = $self->call(
    command => "check_login_token",
    username => $username,
    token => $token
  );

  return $response->{result} eq JSON::true;
}

sub update_user_password {
  my ($self, $username, $password) = @_;

  my $response = $self->call(
    command => "update_user_password",
    username => $username,
    password => $password
  );

  my $ret = $response->{result} eq JSON::true;

  if ($ret) {
    my $statement = $DB->prepare("UPDATE users SET token = NULL WHERE username = ?");
    # Consciously ignore errors.
    $statement->execute($username);
  }

  return $ret;
}

sub set_feature {
  my ($self, $username, $feature, $value) = @_;

  my $response = $self->call(
    command => "set_feature",
    username => $username,
    feature => $feature,
    value => $value,
  );

  return $response->{result} eq JSON::true;
}

1;
