#!/usr/bin/perl -w

use strict;
use warnings;

use CGI;
use CGI::Session;
use CGI::Template;

my $request = new CGI();
my $template = new CGI::Template();

my $sessid = $request->cookie("sessid") || undef;
if ($sessid) {
  my $session = new CGI::Session("driver:File", $sessid, {Directory=>'/tmp'});
  if ($session->param("user_name")) {
    print $request->redirect("profile.pl");
    return;
  }
}

print $template->header();
print $template->content();
