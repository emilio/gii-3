use lib '/etc/sysadmin-app/lib';
use strict;
use warnings;
use Api::Client;

use Test::Most tests => 11, 'die';

my $client = new Api::Client();

ok $client->user_exists("root"), "User root exists";

my $weird = "fwafewawi_should_not_exist";
ok !$client->user_exists($weird), "$weird should not exist";

ok $client->create_user($weird, "password", "alumn"), "Should be able to create an user";

ok $client->user_exists($weird), "Now $weird should exist";

ok $client->group_exists("alumns"), "\"alumns\" group should exist";

my ($correct_login, $token) = $client->check_login($weird, "wrong_password");
ok !$correct_login, "Should not be able to log in with wrong pass";

($correct_login, $token) = $client->check_login($weird, "password");
ok $correct_login, "Should be able to log in";
ok length($token) > 1, "Token should be something";
ok $client->check_login_token($weird, $token), "Token validation should work";

($correct_login, $token) = $client->check_login($weird . "more_weirdness", "password");
ok !$correct_login, "Shouldn't crash with not logged in user";

ok $client->delete_user($weird), "Should be able to delete a user";
