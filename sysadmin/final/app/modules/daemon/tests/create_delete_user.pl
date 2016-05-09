use lib '/etc/sysadmin-app/lib';
use strict;
use warnings;
use Api::Client;
use Data::Dumper;

use Test::Most tests => 20, 'die';

my $client = new Api::Client();

ok $client->user_exists("root"), "User root exists";

my $weird = "fwafewawi_should_not_exist";
ok !$client->user_exists($weird), "$weird should not exist";

ok $client->create_user($weird, "password", "alumn"), "Should be able to create an user";

ok $client->user_exists($weird), "Now $weird should exist";

ok $client->group_exists("alumns"), "\"alumns\" group should exist";

ok $client->user_in_group($weird, "alumns"), "$weird should be in \"alumns\"";

my ($correct_login, $token, $groups) = $client->check_login($weird, "wrong_password");
ok !$correct_login, "Should not be able to log in with wrong pass";

($correct_login, $token, $groups) = $client->check_login($weird, "password");
ok $correct_login, "Should be able to log in";
ok length($token), "Token should be something";
ok $client->check_login_token($weird, $token), "Token validation should work";

# print "Got groups: " . join(',', @$groups) . "\n";
ok $groups, "Should belong to at least one group";

($correct_login, $token, undef) = $client->check_login($weird . "more_weirdness", "password");
ok !$correct_login, "Shouldn't crash with not logged in user";

ok $client->delete_user($weird), "Should be able to delete a user";

ok !$client->create_user($weird, "password", "alumn", "invalidemail", "123 av. st"), "Should not be able to create a user with an invalid email";

ok !$client->user_exists($weird), "$weird should not exist";

ok $client->create_user($weird, "password", "alumn", "test\@gmail.com", "123 av. st"), "Should be able to create a user with email and address";

ok $client->user_exists($weird), "$weird should exist";

my %data = $client->get_user_data($weird);
eq_or_diff $data{'email'}, "test\@gmail.com", "Should have the correct email";

ok $client->update_user_data($weird, "hi\@hi.com", $data{"address"}), "Should be able to update it";

my %data = $client->get_user_data($weird);
eq_or_diff $data{'email'}, "hi\@hi.com", "Should have changed email";
eq_or_diff $data{'address'}, "123 av. st", "Should have keeped address";

ok $client->delete_user($weird), "Should be able to delete a user";

my %data = $client->get_user_data($weird);
ok !$data{"email"}, "Data should have been removed";
