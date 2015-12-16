# Makefile:
#	  Compilation and testing routines for this package.
#
# Copyright (C) 2015 Emilio Cobos Álvarez <emiliocobos@usal.es>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

CFLAGS := -std=c99 -Wall -pedantic -D_REENTRANT -D__USE_XOPEN -D_XOPEN_SOURCE=600
CLINKFLAGS := -pthread
PANDOC_FLAGS := --toc --filter pandoc-crossref
UNAME := $(shell uname)

# Workaround for buggy C compiler in required targets (encina and olivo/SunOS)
ifeq ($(shell hostname), encina)
	CC := gcc
endif
ifeq ($(shell hostname), olivo)
	CC := gcc
endif

ifeq ($(UNAME), SunOS)
	CLINKFLAGS := $(CLINKFLAGS) -lsocket -lnsl
	CFLAGS := $(CFLAGS) -DSOLARIS
endif

ifeq ($(UNAME), Linux)
	CFLAGS := $(CFLAGS) -DLINUX
endif

ifeq ($(UNAME), Darwin)
	CFLAGS := $(CFLAGS) -DDARWIN -D_DARWIN_C_SOURCE
endif

TARGET_NAMES := server client
TARGETS := $(patsubst %, target/%, $(TARGET_NAMES))

TARGET_SOURCES := $(patsubst %, src/%, $(TARGET_NAMES:=.c))
COMMON_SOURCES := $(filter-out $(TARGET_SOURCES), $(wildcard src/*.c))

COMMON_OBJS := $(patsubst src/%, target/%, $(COMMON_SOURCES:.c=.o))

TEST_SOURCES := $(wildcard tests/*.c)
TEST_OBJECTS := $(patsubst tests/%.c, target/tests/%.o, $(TEST_SOURCES))

# Paperwork and statement in pdf
DOC_SOURCES := $(wildcard docs/*.md)
DOC_TARGETS := $(DOC_SOURCES:.md=.pdf)

BUNDLE_NAME := bundle

.PHONY: binaries
binaries: $(TARGETS)
	@echo > /dev/null

.PHONY: clean-binaries
clean-binaries:
	$(RM) -r target

.PHONY: release
release: CFLAGS := $(CFLAGS) -O2
release: clean-binaries binaries
	@echo > /dev/null

.PHONY: test
test: target/tests/tests
	@$<

.PHONY: run
run: run/launch-server.sh
	@$<

.PHONY: docs
docs: $(DOC_TARGETS)
	@echo > /dev/null

.PHONY: all
all: binaries test docs
	@echo > /dev/null

.PHONY: bundle
bundle:
	# Ensure everything is passing before bundling everything
	$(MAKE) --quiet clean-binaries binaries test release test docs > /dev/null 2>&1
	git archive HEAD . -o $(BUNDLE_NAME).tar
	tar -r $(DOC_TARGETS) -f $(BUNDLE_NAME).tar
	gzip -f $(BUNDLE_NAME).tar
	git archive HEAD . -o $(BUNDLE_NAME).zip
	zip $(BUNDLE_NAME).zip $(DOC_TARGETS)

.PHONY: clean-bundle
clean-bundle:
	$(RM) $(BUNDLE_NAME).tar.gz

.PHONY: clean-test
clean-test:
	$(RM) -r target/tests

.PHONY: clean-docs
clean-docs:
	$(RM) $(DOC_TARGETS)

.PHONY: clean-logs
clean-logs:
	$(RM) log/*.log

.PHONY: clean
clean: clean-test clean-docs clean-logs clean-binaries clean-bundle
	@echo > /dev/null

.PHONY: autoformat
autoformat:
	for i in `find src -name '*.c'`; do echo "$$i"; clang-format-3.6 "$$i" > "$$i.formatted"; mv "$$i.formatted" "$$i"; done
	for i in `find src -name '*.h'`; do echo "$$i"; clang-format-3.6 "$$i" > "$$i.formatted"; mv "$$i.formatted" "$$i"; done
	for i in `find tests -name '*.c'`; do echo "$$i"; clang-format-3.6 "$$i" > "$$i.formatted"; mv "$$i.formatted" "$$i"; done
	for i in `find tests -name '*.h'`; do echo "$$i"; clang-format-3.6 "$$i" > "$$i.formatted"; mv "$$i.formatted" "$$i"; done

# Target programs
target/%.o: src/%.c src/%.h
	@mkdir -p $(dir $@)
	$(info [cc] $@)
	@$(CC) $(CFLAGS) -c $< -o $@

target/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(info [cc] $@)
	@$(CC) $(CFLAGS) -c $< -o $@

target/%: target/%.o $(COMMON_OBJS)
	@mkdir -p $(dir $@)
	$(info [cc] $@)
	@$(CC) $(CFLAGS) $^ -o $@ $(CLINKFLAGS)


# Tests
target/tests/%.o: CFLAGS := $(CFLAGS) -I src
target/tests/%.o: tests/%.c tests/%.h
	@mkdir -p $(dir $@)
	$(info [cc] $@)
	@$(CC) $(CFLAGS) -c $< -o $@

target/tests/tests: $(TEST_OBJECTS) $(COMMON_OBJS)
	@mkdir -p $(dir $@)
	$(info [cc] $@)
	@$(CC) $(CFLAGS) $^ -o $@

# Docs
%.pdf: %.md
	$(info [doc] $< -> $@)
	@pandoc $(PANDOC_FLAGS) --from=markdown --latex-engine=xelatex --to=latex $< -o $@

