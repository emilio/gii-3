

.PHONY: install
install:
	apt-get install ssmtp postfix
	rm -f /etc/ssmtp/ssmtp.conf
	stow config -t /etc/ssmtp
