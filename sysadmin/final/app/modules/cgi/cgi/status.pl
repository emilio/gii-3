#!/usr/bin/perl -w
use Proc::ProcessTable;
use CGI::Template;

my @SERVICES = (
  "apache2", # This is kind of stupid, the page won't load if apache isn't running.
  "sshd",
  "ftpd",
  "postfix",
  "saslauthd",
  "sysadmin-appd",
);

my $table = new Proc::ProcessTable;

my $status = "";
foreach $service (@SERVICES) {
  my $service_status = "not-running";
  foreach $process (@{$table->table}) {
    if ($process->cmndline =~ /$service/) {
      $service_status = "running";
      last;
    }
  }

  $status .= "<li class=\"$service status-$service_status\">";
  $status .= "$service <span class=\"status\">$service_status</span>";
  $status .= "</li>";
}

my $template = new CGI::Template();

print $template->header();
print $template->content(
  STATUS => $status,
);
