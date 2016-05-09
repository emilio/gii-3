#!/usr/bin/perl -w
use Proc::ProcessTable;
use CGI::Template;
use Net::FTP;

sub proc_exists {
  my ($table, $name) = @_;

  foreach $p (@{$table->table}) {
    return 1 if $p->cmndline =~ /$name/;
  }
  return 0;
}

my $table = new Proc::ProcessTable;

my %services = ();

my $ftp_conn = new Net::FTP('127.0.0.1');
$services{'ftp'} = $ftp_conn ? 1 : 0;
$ftp_conn->quit() if $ftp_conn;

$services{'sshd'} = proc_exists($table, "sshd");
$services{'apache2'} = proc_exists($table, "apache2");
$services{'postfix'} = proc_exists($table, "postfix");
$services{'sysadmin-appd'} = proc_exists($table, "sysadmin-app");


my $status = "";
foreach $service (sort(keys %services)) {
  my $service_status = $services{$service} ? "running": "not-running";

  $status .= "<tr class=\"$service status-$service_status\">";
  $status .= "  <td class=\"name\">$service</td>";
  $status .= "  <td class=\"status\">$service_status</td>";
  $status .= "</tr>";
}

my $template = new CGI::Template();

print $template->header();
print $template->content(
  STATUS => $status,
);
