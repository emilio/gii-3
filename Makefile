# Simple recursive Makefile

MAKEABLE_DIRS := $(dir $(wildcard */Makefile))

.PHONY: all
all: $(MAKEABLE_DIRS)
	@echo > /dev/null

%/: force
	$(MAKE) -C $@

# A dummy rule to always build targets that depends on them
.PHONY: force
force:
	@true

