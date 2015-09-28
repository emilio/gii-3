JAVA := java
JAVAC := javac

ROOT := $(dir $(CURDIR)/$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))
JADE_ZIP := $(ROOT)jade.zip

JAVAC_FLAGS := $(JAVAC_FLAGS) -g -cp $(abspath $(ROOT)lib/jade.jar):.
JADE_FLAGS :=

# We add the current dir to the classpath to allow booting JADE with
# Agents there
JAVA_FLAGS := $(JAVA_FLAGS) -cp $(abspath $(ROOT)lib/jade.jar):.

# Note we don't use := to allow expanding later
JADE_BOOT_AGENTS = java $(JAVA_FLAGS) jade.Boot $(JADE_FLAGS) -agents

JADE_RUN_LOCAL_AGENT := java $(JAVA_FLAGS) jade.Boot $(JADE_FLAGS) -main false -host 127.0.0.1


.PHONY: deps
deps: $(ROOT)lib/jade.jar
	@echo > /dev/null

$(ROOT)lib/jade.jar:
	$(info [DEPS] $@)
	@mkdir -p $(dir $@)
	@# A repo would be nuts
	wget http://jade.tilab.com/dl.php?file=JADE-bin-4.3.3.zip -O $(JADE_ZIP)
	unzip -j -o $(JADE_ZIP) jade/lib/jade.jar -d $(dir $@)
	@rm $(JADE_ZIP)



%.class: %.java | deps
	$(info [BUILD] $@ <- $<)
	@javac $(JAVAC_FLAGS) $<

clean-class-files-and-jade:
	find . -name '*.class' -delete
	rm -f APDescription.txt MTPs-Main-Container.txt
