JAVA := java
JAVAC := javac

JAVAC_FLAGS :=
JAVA_FLAGS :=

TARGET := ExampleAgent

.PHONY: all
all: $(TARGET).class
	@echo > /dev/null

.PHONY: run
run: $(TARGET).class
	java jade.Boot $(JAVA_FLAGS) -agents hi:$(TARGET)

.PHONY: gui
gui: JAVA_FLAGS := $(JAVA_FLAGS) -gui
gui: run

clean:
	rm -f *.class APDescription.txt MTPs-Main-Container.txt


%.class: %.java
	javac $(JAVAC_FLAGS) $<
