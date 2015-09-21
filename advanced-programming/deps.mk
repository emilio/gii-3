
DEPS_ROOT := $(dir $(CURDIR)/$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))
JADE_ZIP := $(DEPS_ROOT)jade.zip

.PHONY: deps
deps: $(DEPS_ROOT)lib/jade.jar
	@echo > /dev/null

$(DEPS_ROOT)lib/jade.jar:
	mkdir -p $(dir $@)
	# A repo would be nuts
	wget http://jade.tilab.com/dl.php?file=JADE-bin-4.3.3.zip -O $(JADE_ZIP)
	unzip -j -o $(JADE_ZIP) jade/lib/jade.jar -d $(dir $@)
	rm $(JADE_ZIP)

