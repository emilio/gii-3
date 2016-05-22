
.PHONY: install
install:
	apt-get install ruby rubygems
	gem install jekyll --debug
	mkdir -p /etc/skel
	mkdir -p /etc/skel/apuntes
	cp -rf files/* /etc/skel
	cp bind-shared-folder-crontab /etc/cron.d/bind-shared-folder-crontab
