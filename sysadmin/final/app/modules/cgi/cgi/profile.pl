#!/usr/bin/perl -w
use lib '/etc/sysadmin-app/lib';
use strict;
use warnings;

use CGI;
use CGI::Session;
use CGI::Template;

use Api::Client;

my $request = new CGI();
my $template = new CGI::Template();
my $new_cookie;

my $sessid = $request->cookie("sessid") || undef;
my $session = new CGI::Session("driver:File", $sessid, {Directory=>'/tmp'});
if (not defined $sessid) {
  $new_cookie = $request->cookie(-name => "sessid",
                                 -value => $session->id());
}
$sessid = $session->id();

my $user_name = $session->param("user_name");
my $login_token = $session->param("login_token");

my $api_client = new Api::Client();

# Should be logged in, but could be trying now via POST
if (!$user_name or !$login_token) {
  $user_name = $request->param("user_name");
  my $password = $request->param("password");

  if ($request->request_method ne "POST" or !$user_name or !$password) {
    print $request->redirect("login.pl");
    exit 0;
  }

  my ($correct_login, $token) = $api_client->check_login($user_name, $password);
  if (!$correct_login) {
    $template->error("Login error, re-check your credentials");
  }

  $session->param("user_name", $user_name);
  $session->param("login_token", $token);
  $login_token = $token;
}

# This is just an extra safety check
if (!$api_client->check_login_token($user_name, $login_token)) {
  $session->clear(["user_name", "login_token"]);
  $template->error("invalid login token");
}

# my $user = $api_client->get_user_info($user_name);

# TODO: process updates
if ($request->request_method eq "POST") {
}

if (defined $new_cookie) {
  print $template->header(-cookie => $new_cookie);
} else {
  print $template->header();
}

print $template->content(
  SELF => $request->request_uri,
  USER_NAME => $user_name,
  SESSID => $sessid
);
