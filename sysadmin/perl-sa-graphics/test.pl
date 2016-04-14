use strict;
use warnings;

use Email::Send::SMTP::Gmail;
use Excel::Writer::XLSX;

if (@ARGV != 1) {
	print "Usage: $0 [file]\n";
	exit 1;
}

my $file = $ARGV[0];
open my $data, $file or die "Open failed: $!";

# Skip the first line
my $_skip_first_line = <$data>;

my $workbook  = Excel::Writer::XLSX->new('out.xlsx');
my $worksheet = $workbook->add_worksheet();

my $counter = 1;
while (my $line = <$data>) {
  my @array = split(/\s+/, $line);
  $worksheet->write("A$counter", $array[6]); # Command
  $worksheet->write("B$counter", $array[1]); # Uses
  $counter++;
}
$counter--;

my $chart = $workbook->add_chart( type => 'pie', embedded => 1);

# $counter = 3;
print "$counter\n";


$chart->add_series(
  categories => ['Sheet1', 0, $counter, 0, 0],
  values => ['Sheet1', 0, $counter, 1, 1],
);

$chart->set_title( name => 'Popular Commands');
$chart->set_size( width => 720, height => 480 );

$worksheet->insert_chart('C1', $chart);
$workbook->close();

exit 1;

my $mail = Email::Send::SMTP::Gmail->new(
  -smtp => 'gmail.com',
  -login => $ENV{'GMAIL_USER'},
  -pass => $ENV{'GMAIL_PASS'});

my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime();
my $subject = "Subject: Stats $mday/$mon/$year";

$mail->send(
  -to => 'ecoal95@gmail.com',
  -subject => $subject,
  -verbose => '1',
  -body => 'Just testing it',
  -attachments => 'full_path_to_file');

$mail->bye;
