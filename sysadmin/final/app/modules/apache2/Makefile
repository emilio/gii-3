VHOSTS_DIR := /var/www/vhosts
HOSTNAME ?= localhost

.PHONY: install
install:
	apt-get install stow apache2 php5 php5-mysql php5-mcrypt php5-curl php5-pgsql postgresql postgresql-client php5-gd
	a2enmod rewrite
	a2enmod cgi
	a2enmod ssl
	stow config -t /etc/apache2/conf-enabled
	rm /etc/apache2/sites-enabled/*.conf
	stow sites -t /etc/apache2/sites-enabled
	service apache2 restart
