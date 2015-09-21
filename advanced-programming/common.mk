JAVA := java
JAVAC := javac

ROOT := $(dir $(CURDIR)/$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))

JAVAC_FLAGS := -cp $(abspath $(ROOT)lib/jade.jar)
JAVA_FLAGS := -cp $(abspath $(ROOT)lib/jade.jar)
