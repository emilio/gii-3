VHOSTS_DIR := /var/www/vhosts
HOSTNAME ?= localhost

.PHONY: install
install:
	apt-get install apache2
	stow config /etc/apache2
	mkdir $(VHOSTS_DIR)
	chown -R www-data:www-data $(VHOSTS_DIR)
