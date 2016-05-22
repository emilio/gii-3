#!/usr/bin/perl -w
use lib '/etc/sysadmin-app/lib';
use strict;
use warnings;

use CGI;
use CGI::Session;
use CGI::Template;
use HTML::Entities;
use Email::Valid;

use Api::Client;

my $request = new CGI();
my $template = new CGI::Template();
my $api_client = new Api::Client();

my $sessid = $request->cookie("sessid") || undef;
my $success = "";

if ($sessid) {
  my $session = new CGI::Session("driver:File", $sessid, {Directory=>'/tmp'});
  if ($session->param("user_name")) {
    print $request->redirect("profile.pl");
    return;
  }
}

if ($request->request_method eq "POST") {
  my $username = $request->param("user_name");
  if ($username) {
    my $remember_url = "http";
    $remember_url .= "s" if $ENV{HTTPS} eq "on";
    $remember_url .= "://" . $ENV{SERVER_NAME};
    $remember_url .= ":" . $ENV{SERVER_PORT} unless $ENV{SERVER_PORT} eq "80";
    $remember_url .= "/cgi/regenpass.pl";

    $api_client->generate_remember_password_token_and_send_email($username, $remember_url);
    $success = "If the user exists there's now an email in his inbox.";
  }
}

print $template->header();
print $template->content(
  SUCCESS => $success,
);
