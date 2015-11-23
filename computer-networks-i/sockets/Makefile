CFLAGS := -std=c99 -Wall -pedantic
CLINKFLAGS := -pthread
PANDOC_FLAGS := --toc --filter pandoc-crossref

TARGET_NAMES := server client
TARGETS := $(patsubst %, target/%, $(TARGET_NAMES))

TARGET_SOURCES := $(patsubst %, src/%, $(TARGET_NAMES:=.c))
COMMON_SOURCES := $(filter-out $(TARGET_SOURCES), $(wildcard src/*.c))

COMMON_OBJS := $(patsubst src/%, target/%, $(COMMON_SOURCES:.c=.o))

TEST_SOURCES := $(wildcard tests/*.c)
TEST_OBJECTS := $(patsubst tests/%.c, target/tests/%.o, $(TEST_SOURCES))

# Paperwork and statement in pdf
DOC_SOURCES := $(wildcard *.md)
DOC_TARGETS := $(DOC_SOURCES:.md=.pdf)

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

.PHONY: clean-test
clean-test:
	$(RM) -r target/tests

.PHONY: docs
docs: $(DOC_TARGETS)
	@echo > /dev/null

.PHONY: clean-docs
clean-docs:
	$(RM) $(DOC_TARGETS)

.PHONY: all
all: binaries test docs
	@echo > /dev/null

.PHONY: clean
clean: clean-test clean-docs clean-binaries
	@echo > /dev/null

.PHONY: autoformat
autoformat:
	for i in `find src -name '*.c'`; do echo "$$i"; clang-format-3.6 "$$i" > "$$i.formatted"; mv "$$i.formatted" "$$i"; done
	for i in `find src -name '*.h'`; do echo "$$i"; clang-format-3.6 "$$i" > "$$i.formatted"; mv "$$i.formatted" "$$i"; done

# Target programs
target/%.o: src/%.c src/%.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

target/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

target/%: target/%.o $(COMMON_OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(CLINKFLAGS)


# Tests
target/tests/%.o: CFLAGS := $(CFLAGS) -I src
target/tests/%.o: tests/%.c tests/%.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

target/tests/tests: $(TEST_OBJECTS) $(COMMON_OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@

# Docs
%.pdf: %.md
	$(info [DOC] $< -> $@)
	@pandoc $(PANDOC_FLAGS) --from=markdown --latex-engine=xelatex --to=latex $< -o $@

