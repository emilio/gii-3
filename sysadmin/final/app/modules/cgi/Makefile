# This installs our custom cgi scripts for administration
# in apache's cgi-bin directory
TARGET := /var/www/html/cgi

.PHONY: install
install:
	rm -rf $(TARGET)
	mkdir -p $(TARGET)
	cp -r cgi/* $(TARGET)
	cp -r cgi/.htaccess $(TARGET)
	chown -R www-data:www-data $(TARGET)
