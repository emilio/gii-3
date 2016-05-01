# TODO: Add init script and systemd unit
.PHONY: install
install:
	$(RM) /etc/sysadmin-app
	ln -s $(shell pwd) /etc/sysadmin-app
	$(RM) /etc/init.d/sysadmin-appd
	([ -d /etc/init.d ] && ln -s /etc/sysadmin-app/init.d/sysadmin-appd /etc/init.d) || true
	$(RM) /etc/systemd/system/sysadmin-appd.service
	([ -d /etc/systemd/system ] && ln -s /etc/sysadmin-app/systemd/sysadmin-appd.service /etc/systemd/system/) || true
	service sysadmin-appd restart 2>/dev/null || systemctl restart sysadmin-appd.service
	@echo > /dev/null

.PHONY: test
test:
	bash tests/run.sh
