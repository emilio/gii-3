POSTGRES_USER ?= postgres

# This installs a cron script used to manage our users
# and all the stuff.
.PHONY: install
install:
	su -c "pgsql postgres -f $(shell pwd)/setup.sql" postgres
	su -c "pgsql app -f $(shell pwd)/schema.sql" postgres
	crontab -l | { cat; echo "* * * * * $(shell pwd)/manage_requests.pl"; } | crontab -
