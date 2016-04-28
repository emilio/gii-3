use strict;
use warnings;

# TODO: don't force this and read it from the environment
use DBI;

my $dbh = DBI->connect("dbi:Pg:dbname=app", "app", "12345678")
          or die("Couldn't connect to app database" . DBI->errstr);

my $statement = $dbh->prepare("SELECT * FROM user_requests")
                or die("Couldn't prepare user requests: " . $dbh->errstr);

$statement->execute()
                or die("Couldn't fetch user requests: " . $dbh->errstr);

if ($statement->rows == 0) {
  print "No users found"
}

while (my @data = $statement->fetchrow_array()) {
  my $user = $data[0];
  my $pass = $data[1];

  print "$user: $pass"
}
