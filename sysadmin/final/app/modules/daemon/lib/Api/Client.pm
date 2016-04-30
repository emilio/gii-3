package Api::Client;
use Api::Common;
use JSON;

use IO::Socket::INET;

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

  my $response = $self->call(
      command => "delete_user",
      username => $username
  );

  return $response->{result} eq JSON::true;
}

sub create_user {
  my ($self, $username, $password, $type) = @_;

  my $response = $self->call(
      command => "create_user",
      username => $username,
      password => $password,
      type => $type,
  );

  return $response->{result} eq JSON::true;
}

sub check_login {
  my ($self, $username, $password) = @_;

  my $response = $self->call(
      command => "login",
      username => $username,
      password => $password
  );

  return $response->{result} eq JSON::true;
}

1;
