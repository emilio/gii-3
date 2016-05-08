#!/usr/bin/perl -w

use strict;
use warnings;

use CGI;
use CGI::Session;
use CGI::Template;

my $request = new CGI();
my $template = new CGI::Template();

print $template->header();
print $template->content(NAME => $request->param("name"));
