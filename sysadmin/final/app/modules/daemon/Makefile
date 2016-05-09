APP_DB_USER ?= sysadminapp
APP_DB_NAME ?= sysadminapp
POSTGRES_USER ?= postgres

.PHONY: install
install:
	$(RM) /etc/sysadmin-app
	ln -s $(shell pwd) /etc/sysadmin-app
	$(RM) /etc/init.d/sysadmin-appd
	([ -d /etc/init.d ] && ln -s /etc/sysadmin-app/init.d/sysadmin-appd /etc/init.d) || true
	$(RM) /etc/systemd/system/sysadmin-appd.service
	([ -d /etc/systemd/system ] && ln -s /etc/sysadmin-app/systemd/sysadmin-appd.service /etc/systemd/system/) || true
	service sysadmin-appd restart 2>/dev/null || systemctl restart sysadmin-appd.service
	$(info WARNING: make sure the data you introduce here \
		     matches the contents of the sysadminapprc)
	su -c "dropdb -i $(APP_DB_NAME)" postgres || true
	su -c "dropuser -i $(APP_DB_USER)" postgres || true
	su -c "createuser -U $(POSTGRES_USER) -e -d -P $(APP_DB_USER)" postgres
	su -c "createdb -E UTF8 -O $(APP_DB_USER) $(APP_DB_NAME)" postgres
	psql -U $(APP_DB_USER) -d $(APP_DB_NAME) -a -f /etc/sysadmin-app/schema.sql
	@echo > /dev/null

.PHONY: test
test:
	bash tests/run.sh
