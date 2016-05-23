# A note about the roundcube installation:
#
# Roundcube by default doesn't let to send emails to localhost or similar.
# You can workaround this sending to localhost.localdomain
.PHONY: install
install:
	apt-get install roundcube roundcube-pgsql
	# Already symlinked by roundcube.
	# cp /etc/roundcube/apache.conf /etc/apache2/conf-enabled/roundcube.conf
	stow -d config apache -t /etc/apache2/conf-enabled
	$(RM) /etc/roundcube/config.inc.php
	stow -d config roundcube -t /etc/roundcube
	service apache2 restart
