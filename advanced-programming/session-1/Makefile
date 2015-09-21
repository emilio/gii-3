include ../common.mk

TARGETS := ExampleAgent
TARGET_CLASS := $(patsubst %, %.class, $(TARGETS))

.PHONY: all
all: notes.pdf $(TARGET_CLASS)
	@echo > /dev/null

include ../../session.mk
include ../deps.mk

.PHONY: gui
run: $(TARGET_CLASS)
	java jade.Boot $(JAVA_FLAGS) -agents hi:ExampleAgent

.PHONY: gui
gui: JAVA_FLAGS := $(JAVA_FLAGS) -gui
gui: run

clean:
	rm -f *.class APDescription.txt MTPs-Main-Container.txt

%.class: %.java | deps
	@echo "[BUILD] $@ <- $<"
	javac $(JAVAC_FLAGS) $<
