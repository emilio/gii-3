# TODO: Add init script and systemd unit
.PHONY: install
install:
	$(RM) /etc/sysadmin-app
	ln -s $(shell pwd) /etc/sysadmin-app
	$(RM) /etc/init.d/sysadmin-appd
	ln -s /etc/sysadmin-app/init.d/sysadmin-appd /etc/init.d
	$(RM) /etc/systemd/system/sysadmin-appd.service
	ln -s /etc/sysadmin-app/systemd/sysadmin-appd.service /etc/systemd/system/
	service sysadmin-appd restart
	@echo > /dev/null

.PHONY: test
test:
	bash tests/run.sh
