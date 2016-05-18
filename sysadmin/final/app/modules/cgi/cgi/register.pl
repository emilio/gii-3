#!/usr/bin/perl -w
use lib '/etc/sysadmin-app/lib';
use strict;
use warnings;

use CGI;
use CGI::Session;
use CGI::Template;
use CGI::Cookie;
use Email::Valid;

use Api::Client;

my $request = new CGI();
my $template = new CGI::Template();
my $sessid = $request->cookie("sessid") || undef;
if ($sessid) {
  my $session = new CGI::Session("driver:File", $sessid, {Directory=>'/tmp'});
  if ($session->param("user_name")) {
    print $request->redirect("profile.pl");
    exit(0);
  }
}

# TODO: Actually validate and register the user here
if ($request->request_method eq "POST") {
  my $user_name = $request->param("user_name");
  my $password = $request->param("password");
  my $password_confirmation = $request->param("password_confirmation");
  my $email = $request->param("email");
  my $address = $request->param("address");
  my $type = $request->param("type");

  if (!$user_name ||
      !$password ||
      !$password_confirmation ||
      !$email ||
      !$address ||
      !$type) {
    $template->error("Empty parameter");
  }

  if ($password ne $password_confirmation) {
    $template->error("Password mismatch");
  }

  if (!Email::Valid->address($email)) {
    $template->error("Invalid email address");
  }

  if ($user_name !~ /^[A-Za-z0-9]+$/) {
    $template->error("Invalid username, must be alphanumeric");
  }

  if ($type ne "teacher" and $type ne "alumn") {
    $template->error("Invalid type");
  }

  my $client = new Api::Client();
  if ($client->user_exists($user_name)) {
    $template->error("$user_name already exists");
  }

  # We only create alumns by default
  if (!$client->create_user($user_name, $password, $type, $email, $address)) {
    $template->error("Failed to create user");
  }

  # TODO: Send email with terms and conditions, welcome, etc...
  print $request->redirect("welcome.pl?name=" . $user_name);
  exit(0)
}

print $template->header();
print $template->content();
