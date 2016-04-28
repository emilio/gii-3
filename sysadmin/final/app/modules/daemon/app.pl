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
my $TEST_MODE = 1;

if ($DAEMONIZE) {
  Proc::Daemon::Init;
}

my $CONTINUE = 1;
$SIG{TERM} = sub { $CONTINUE = 0 };

my $port = 7777;

# auto-flush on socket
# $| = 1;

my $socket = new IO::Socket::INET (
  LocalHost => '127.0.0.1',
  LocalPort => $port,
  Proto => 'tcp',
  Listen => 5,
  Reuse => 1
) or die("couldn't create socket");

print "server waiting for client connection on port $port\n";

sub check_user_login {
  my ($username, $password) = @_;

  if (!check_user_exists($username)) {
    return 0;
  }

  my $user = Linux::usermod->new($username);

  # TODO: Use PAM or something like that?
  my $encrypted_password = $user->get('password');
  my $hash = crypt($password, $encrypted_password);

  # Woohoo!
  return $hash eq $encrypted_password;
}

sub check_user_exists {
  my ($username) = @_;
  my %users = Linux::usermod->users();

  return exists $users{$username};
}

sub do_action {
  my ($socket, $command) = @_;

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
      $result = check_user_exists($command->{username});
    }
  }

  if ($result) {
    $socket->send(encode_json({ result => JSON::true }));
  } else {
    $socket->send(encode_json({ result => JSON::false }));
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
