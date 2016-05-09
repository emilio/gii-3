#!/usr/bin/perl -w
use lib '/etc/sysadmin-app/lib';
use strict;
use warnings;

use CGI;
use CGI::Session;
use CGI::Template;
use HTML::Entities;

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

if ($request->request_method eq "POST") {
  # Delete the user if he requests so
  if ($request->param("action") eq "Delete account") {
    $api_client->delete_user($user_name);
    print $request->redirect("logout.pl");
    exit 0;
  }
}

if (defined $new_cookie) {
  print $template->header(-cookie => $new_cookie);
} else {
  print $template->header();
}

my %data = $api_client->get_user_data($user_name);

print $template->content(
  EMAIL => $data{"email"},
  ADDRESS => encode_entities($data{"address"}),
  USER_NAME => $user_name,
  SESSID => $sessid
);
