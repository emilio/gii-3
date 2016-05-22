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
my $api_client = new Api::Client();

my $token = $request->param("token")
              or $template->error("No token");
my $username = $request->param("username")
                 or $template->error("No username");

my %data = $api_client->get_user_data($username)
             or $template->error("Invalid request");

if ($data{'token'} ne $token) {
  $template->error("Invalid token");
}

if ($request->request_method eq "POST") {
  my $password = $request->param("password");
  my $password_confirmation = $request->param("password_confirmation");

  if (!$password or !$password_confirmation) {
    $template->error("Missing password");
  } elsif ($password ne $password_confirmation) {
    $template->error("Password mismatch");
  } elsif (!$api_client->update_user_password($username, $password)) {
    $template->error("Unknown error while updating the password");
  } else {
    print $request->redirect("profile.pl");
    return;
  }
  $template->error("this should be unreachable");
}

print $template->header();
print $template->content(
  TOKEN => $token,
  USER_NAME => $username,
);
