# Simple recursive Makefile

MAKEABLE_DIRS := $(dir $(wildcard */Makefile))
MAKE_ARGS :=

.PHONY: all
all: $(MAKEABLE_DIRS)
	@echo > /dev/null

.PHONY: clean
clean: MAKE_ARGS := clean
clean: all

%/: force
	$(MAKE) -C $@ $(MAKE_ARGS) || true

# A dummy rule to always build targets that depends on them
.PHONY: force
force:
	@true

