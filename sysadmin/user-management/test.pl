#!/usr/bin/env perl
# use strict;
use Data::Dumper;
use Linux::usermod;
use File::Path;
use Quota;

if (@ARGV != 2) {
	print "Usage: $0 username password block-quota\n";
	exit 1;
}

my $username = $ARGV[0];
my $password = $ARGV[1];

my $linux_users = Linux::usermod->users();

print $linux_users{$username} . "\n";
print Dumper($linux_users);
print "$_ $linux_users{$_}\n" for (keys %linux_users);
exit 1;

Linux::usermod->add($username, $password);
my $user = Linux::usermod->new($username);
my $uid = $user->get("uid");

print Quota::query("/dev/sda5", $uid);
my ($bc, $bc, $bs, $bt, $ic, $is, $ih, $it) = Quota::query("/dev/sda5", $user->get("uid"));
