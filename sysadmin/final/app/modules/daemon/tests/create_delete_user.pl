use lib '/etc/sysadmin-app/lib';
use strict;
use warnings;
use Api::Client;

use Test::Most tests => 8, 'die';

my $client = new Api::Client();

ok $client->user_exists("root"), "User root exists";

my $weird = "fwafewawi_should_not_exist";
ok !$client->user_exists($weird), "$weird should not exist";

ok $client->create_user($weird, "password", "alumn"), "Should be able to create an user";

ok $client->user_exists($weird), "Now $weird should exist";

ok $client->group_exists("alumns"), "\"alumns\" group should exist";

ok !$client->check_login($weird, "wrong_password"), "Should not be able to log in with wrong pass";

ok $client->check_login($weird, "password"), "Should be able to login";

ok $client->delete_user($weird), "Should be able to delete a user";
