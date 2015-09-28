
TARGETS := ExampleAgent
TARGET_CLASS := $(patsubst %, %.class, $(TARGETS))

.PHONY: all
all: notes.pdf $(TARGET_CLASS)
	@echo > /dev/null

include ../common.mk
include ../../session.mk

.PHONY: run
run: $(TARGET_CLASS)
	$(JADE_BOOT_AGENTS) hi:ExampleAgent

.PHONY: gui
gui: JAVA_FLAGS := $(JAVA_FLAGS) -gui
gui: run

clean: clean-class-files-and-jade
	@echo > /dev/null

