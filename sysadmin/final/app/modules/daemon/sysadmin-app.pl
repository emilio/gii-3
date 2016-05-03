#!/usr/bin/perl
use lib '/etc/sysadmin-app/lib';

use strict;
use warnings;
use Proc::Daemon;
use Api::Server;
use Getopt::Long;

# TODO: Parse CLI args to change these,
# and make a test suite.
my $DAEMONIZE = 0;
my $TEST_MODE = 1;


GetOptions("daemonize" => \$DAEMONIZE,
           "test" => \$TEST_MODE)
           or die("Error in command line arguments\n");

if ($DAEMONIZE) {
  Proc::Daemon::Init;
}

my $CONTINUE = 1;
$SIG{TERM} = sub { $CONTINUE = 0 };

my $server = new Api::Server();

if ($TEST_MODE) {
  $server->hard_fail();
}

while ($CONTINUE) {
  $server->wait_and_dispatch_command();
}

$server->close();
