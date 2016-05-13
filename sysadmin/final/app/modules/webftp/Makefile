MFTP_DIR := /var/www/html/mftp

# TODO: Quota + edit fstab + lock users to use $HOME
.PHONY: install
install:
	apt-get install vsftpd
	cp vsftpd.conf /etc/vsftpd.conf
	service vsftpd restart
	$(RM) -r $(MFTP_DIR) master.zip
	wget https://github.com/MonstaApps/Monsta-FTP/archive/master.zip
	unzip master.zip
	mv Monsta-FTP-master $(MFTP_DIR)
	cp config.php $(MFTP_DIR)
	chown -R www-data:www-data $(MFTP_DIR)
