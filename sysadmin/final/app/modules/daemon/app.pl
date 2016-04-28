#!/usr/bin/perl

use strict;
use warnings;
use JSON;
use IO::Socket::INET;
use Proc::Daemon;
use Try::Tiny;

# Proc::Daemon::Init;

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

sub do_action {
  my ($socket, $command) = @_;

  my $error = 0;
  try {
    $command = decode_json($command);
  } catch {
    print "JSON error caught: $command";
    $error = 1;
  };

  if ($error) {
    $socket->send("error");
    return;
  }

  my $reencoded = encode_json($command);
  print "command: $reencoded";
  $socket->send($reencoded);
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
  do_action($client_socket, $data);

  # notify client that response has been sent
  shutdown($client_socket, 1);
}

$socket->close();
