TARGET :=	bin/main
TESTS := bin/tests/main

CXX := mpic++
CC := mpicc

# crypt("10", "aa");
TEST_PASSWORDS := aal9/sIHZQyhA

COMMON_FLAGS := -Wall -pedantic

CFLAGS := $(COMMON_FLAGS) -std=c99 -pedantic
CXXFLAGS := $(COMMON_FLAGS) -std=c++11
CXXLINKFLAGS := -lcrypt

NP ?= 4

OBJECTS := $(patsubst src/%, obj/%, \
					   $(patsubst %.c, %.o, $(wildcard src/*.c)) \
						 $(patsubst %.cpp, %.o, $(wildcard src/*.cpp)))

.PHONY: all
all: $(TARGET)
	@echo > /dev/null

.PHONY: clean
clean:
	$(RM) -r obj bin

.PHONY: test
test: $(TESTS)
	mpirun -np $(NP) $<

.PHONY: run
run: $(TARGET)
	mpirun -np $(NP) $(TARGET) $(TEST_PASSWORDS)

obj/%.o: src/%.cpp src/%.hpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/%.o: src/%.c src/%.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

bin/%: obj/%.o $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(CXXLINKFLAGS)

bin/tests/%: tests/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -Isrc $(CXXFLAGS) $^ -o $@ $(CXXLINKFLAGS)

