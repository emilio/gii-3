MOODLE_DOWNLOAD_URL := https://download.moodle.org/download.php/direct/stable30/moodle-latest-30.tgz
MOODLE_TARGET_DIR ?= /var/www/moodle
MOODLE_DATA_DIR ?= /var/moodle
MOODLE_DB_USER ?= moodle
MOODLE_DB_NAME ?= moodle
POSTGRES_USER ?= postgresql

.PHONY: install
install:
	# Setup target dir
	curl $(MOODLE_DOWNLOAD_URL) | tar -xzvf - -o $(MOODLE_TARGET_DIR)
	chown -R root $(MOODLE_TARGET_DIR)
	chmod -R 0755 $(MOODLE_TARGET_DIR)
	find $(MOODLE_TARGET_DIR) -type f -exec chmod 0644 {} \;
	chown www-data $(MOODLE_TARGET_DIR)
	# Setup data dir
	mkdir -p $(MOODLE_DATA_DIR)
	chmod 0777 $(MOODLE_DATA_DIR)
	# Setup the PostgreSQL database
	createuser -U $(POSTGRES_USER) -P $(MOODLE_DB_USER)
	createdb -U $(MOODLE_DB_USER) $(MOODLE_DB_NAME)
	# TODO: php install.php
