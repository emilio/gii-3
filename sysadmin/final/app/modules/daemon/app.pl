#!/usr/bin/perl

use strict;
use warnings;
use JSON;
use IO::Socket::INET;
use Proc::Daemon;
use Switch;
use Try::Tiny;
use Linux::usermod;

# TODO: Parse CLI args to change these,
# and make a test suite.
my $DAEMONIZE = 0;
my $TEST_MODE = 0;

if ($DAEMONIZE) {
  Proc::Daemon::Init;
}

my $CONTINUE = 1;
$SIG{TERM} = sub { $CONTINUE = 0 };

my $port = 7777;


# auto-flush on socket
$| = 1;
# creating a listening socket
my $socket = new IO::Socket::INET (
  LocalHost => '127.0.0.1',
  LocalPort => $port,
  Proto => 'tcp',
  Listen => 5,
  Reuse => 1
) or die("couldn't create socket");

print "server waiting for client connection on port $port\n";

sub check_user_login {
  my ($socket, $username, $password) = @_;

  my $user = Linux::usermod->new($username);
  if (!$user) {
    print "User not found: $username\n";
    $socket->send(encode_json({ result => JSON::false }));
    return;
  }

  # TODO: Use PAM or something like that?
  my $encrypted_password = $user->get('password');
  my $hash = crypt($password, $encrypted_password);

  # Woohoo!
  if ($hash eq $encrypted_password) {
    $socket->send(encode_json({ result => JSON::true }));
  } else {
    $socket->send(encode_json({ result => JSON::false }));
  }
}

sub do_action {
  my ($socket, $command) = @_;

  $command = decode_json($command);

  my $reencoded = encode_json($command);
  print "command: $reencoded";

  switch ($command->{command}) {
    case "check" {
      check_user_login($socket,
                       $command->{username},
                       $command->{password});
    }
  }
}

while ($CONTINUE) {
  # waiting for a new client connection
  my $client_socket = $socket->accept();

  # get information about a newly connected client
  my $client_address = $client_socket->peerhost();
  my $client_port = $client_socket->peerport();
  print "connection from $client_address:$client_port\n";

  # read up to 1024 characters from the connected client
  my $data = "";
  $client_socket->recv($data, 1024);

  print "Received: $data";
  my $error = 0;

  if ($TEST_MODE) {
    do_action($client_socket, $data);
  } else {
    try {
      do_action($client_socket, $data);
    } catch {
      print "JSON error caught: $data";
      $error = 1;
    };
  }

  if ($error) {
    $client_socket->send("error");
  }
  # notify client that response has been sent
  shutdown($client_socket, 1);
}

$socket->close();
