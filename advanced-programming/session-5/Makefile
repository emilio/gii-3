
SOURCES := $(wildcard *.adb)
TARGETS := $(SOURCES:.adb=)

all: $(TARGETS)
	@echo > /dev/null

include ../ada.mk

clean: clean-ada-files
	rm -f $(TARGETS)
