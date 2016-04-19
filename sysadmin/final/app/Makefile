CPAN := cpan
PERL_FILES := $(shell find . -type f -name '*.pl')
REQ := requirements.perl.txt

# Need to be kept in the correct order to
# satisfy dependency chains
MODULES := apache2 mail moodle cgi
INSTALLED_MODULES := $(patsubst %, modules/%, $(MODULES:=.installed))

.PHONY: cpan-install
cpan-install:
	while read -r line; do cpan $$line; done < $(REQ)

.PHONY: cpan-dump
cpan-dump: $(REQ)
	@echo > /dev/null

$(REQ): dev/dump-modules $(PERL_FILES)
	@./dev/dump-modules $(shell pwd) $@


.PHONY: install
install: $(INSTALLED_MODULES)
	@echo > /dev/null

# perl $<
%.installed: %
	@make -C $< install
	touch $@
