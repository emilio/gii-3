.PHONY: install
install:
	apt-get install postfix courier-authdaemon courier-imap courier-pop \
	                courier-imap-ssl courier-pop-ssl
	$(RM) /etc/postfix/main.cf
	stow config -t /etc/postfix
	service postfix restart
