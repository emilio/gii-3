VHOSTS_DIR := /var/www/vhosts
HOSTNAME ?= localhost

.PHONY: install
install:
	apt-get install apache2 php5 php5-mysql php5-mcrypt php5-curl php5-pgsql postgresql postgresql-client php5-gd
	a2enmod rewrite
	service apache2 restart
	# stow config /etc/apache2
	mkdir -p $(VHOSTS_DIR)
	chown -R www-data:www-data $(VHOSTS_DIR)
