#!/usr/bin/perl -w

use strict;
use warnings;

use CGI;
use CGI::Template;

my $template = new CGI::Template();

print $template->header();
print $template->content();
