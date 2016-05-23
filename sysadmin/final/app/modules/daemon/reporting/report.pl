use lib '/etc/sysadmin-app/lib';
use strict;
use warnings;
use Api::Client;

my $body = "";
my $subject = "";
my $api_client = new Api::Client();

my ($sec, $min, $hour, $mday, $mon, $year, $wday, $yday, $isdst) = localtime();


$subject .= "[sysadmin-app] Server report for $year/$mon/$mday";

$body .= <<'EOF'
Server Status Report
====================

EOF
;

# This executes the scripts directly since there's no port for perl.
# Well, there's a port for `uptime`, but doing it just for that is bs.
my $uptime = `uptime`;
my $sa = `sa`;
my $sar = `sar`;
$body .= <<'EOF'

Uptime
------

  $uptime

Most used commands
------------------

  $sa

Daily statistics
----------------

  $sar

EOF
;

my $destination = defined($ENV{'MAILTO'}) ? $ENV{'MAILTO'} : 'root@localhost';
$api_client->send_generic_email($destination, $subject, $body);
print "Statistics for $year/$mon/$mday collected!"
