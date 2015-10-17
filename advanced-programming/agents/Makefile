SOURCES := $(shell find src -type f -name '*.java')
TARGETS = $(patsubst src/%, bin/%, $(SOURCES:.java=.class))

all: $(TARGETS)
	@echo > /dev/null

CLASSPATH := $(shell ~/eclipse/eclim -command java_classpath -p agents)

include ../common.mk

.PHONY: run
run: all
	$(JADE_BOOT_AGENTS) teacher:agents.Teacher

.PHONY: run-secondary
run-secondary:
	for i in `seq 1 10`; do $(JADE_RUN_LOCAL_AGENT) "available_alumn$$i:agents.AvailableAlumn" & done

.PHONY: gui
gui: JADE_FLAGS := $(JADE_FLAGS) -gui
gui: run

