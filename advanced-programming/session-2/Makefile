SOURCES := $(shell find . -type f -name '*.java')
TARGETS = $(SOURCES:.java=.class)

all: $(TARGETS)
	@echo > /dev/null

include ../common.mk

.PHONY: run
run: all
	$(JADE_BOOT_AGENTS) hi:Agent

.PHONY: run-secondary
run-secondary:
	$(JADE_RUN_LOCAL_AGENT) agent2:Agent

.PHONY: gui
gui: JADE_FLAGS := $(JADE_FLAGS) -gui
gui: run

