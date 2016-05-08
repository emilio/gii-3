#!/usr/bin/perl -w

use strict;
use warnings;

use CGI;
use CGI::Template;
use CGI::Session;

my $request = new CGI();

my $sessid = $request->cookie("sessid") || undef;
if ($sessid) {
  my $session = new CGI::Session("driver:File", $sessid, {Directory=>'/tmp'});
  $session->clear(["user_name", "login_token"]);
}

print $request->redirect("login.pl");
