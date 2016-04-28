POSTGRES_USER ?= postgres

# This installs a cron script used to manage our users
# and all the stuff.
.PHONY: install
install:
	cp *.sql /tmp
	su -c "psql postgres -f /tmp/setup.sql" postgres
	su -c "psql app -f /tmp/schema.sql" postgres
	crontab -l | { cat; echo "* * * * * perl $(shell pwd)/manage_requests.pl"; } | crontab -
