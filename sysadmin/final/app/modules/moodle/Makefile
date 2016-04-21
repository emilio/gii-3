MOODLE_DOWNLOAD_URL := https://download.moodle.org/download.php/direct/stable30/moodle-latest-30.tgz
MOODLE_TARGET_DIR ?= /var/www/html/moodle
MOODLE_DATA_DIR ?= /var/moodle
MOODLE_DB_USER ?= moodle
MOODLE_DB_NAME ?= moodle
MOODLE_ADMIN_USER ?= admin
MOODLE_ADMIN_PASS ?= admin
POSTGRES_USER ?= postgres

.PHONY: install
install:
	# Setup target dir
	curl $(MOODLE_DOWNLOAD_URL) | tar -xzvf -
	rm -rf $(MOODLE_TARGET_DIR)
	mv moodle $(MOODLE_TARGET_DIR)
	chown -R root $(MOODLE_TARGET_DIR)
	chmod -R 0755 $(MOODLE_TARGET_DIR)
	find $(MOODLE_TARGET_DIR) -type f -exec chmod 0644 {} \;
	chown www-data $(MOODLE_TARGET_DIR)
	# Setup data dir
	mkdir -p $(MOODLE_DATA_DIR)
	chmod 0777 $(MOODLE_DATA_DIR)
	# Setup the PostgreSQL database
	# TODO: Detect if the user exists instead of doing || true
	su -c "dropdb -i $(MOODLE_DB_NAME)" postgres || true
	su -c "dropuser -i $(MOODLE_DB_USER)" postgres || true
	su -c "createuser -U $(POSTGRES_USER) -e -d -P $(MOODLE_DB_USER)" postgres
	su -c "createdb -O $(MOODLE_DB_USER) $(MOODLE_DB_NAME)" postgres
	cd $(MOODLE_TARGET_DIR) && php admin/cli/install.php --dbtype=pgsql \
	  --dbname=$(MOODLE_DB_NAME) \
	  --dbuser=$(MOODLE_DB_USER) \
	  --dataroot=$(MOODLE_DATA_DIR) \
	  --wwwroot=http://$(HOSTNAME)/moodle \
	  --dataroot=$(MOODLE_DATA_DIR) \
	  --adminuser=$(MOODLE_ADMIN_PASS) \
	  --adminpass=$(MOODLE_ADMIN_PASS)
