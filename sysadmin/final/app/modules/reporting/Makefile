
.PHONY: install
install:
	cp report-daily /etc/cron.d/report-daily
	apt-get install acct sysstat
	$(RM) /etc/sysstat/sysstat
	stow sysstat -t /etc/sysstat
	$(RM) /etc/default/sysstat
	stow sysstat-default -t /etc/default
	service sysstat restart
