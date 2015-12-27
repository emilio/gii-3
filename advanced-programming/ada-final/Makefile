SOURCE := final.adb
TARGET := $(SOURCE:.adb=)

.PHONY: all
all: $(TARGET)
	@echo > /dev/null

.PHONY: test
test:
	timeout 15 ./$(TARGET) > test.txt || true
	[ $$(cat test.txt | grep 'ESTABLE\|PELIGRO' | wc -l) -eq 50 ] || (echo "Failure!" && false)
	@echo "Ok!"

%.o: %.adb
	gnatmake $<

%: %.adb
	gnatmake $<

clean-ada-files:
	find . -type f -name '*.o' -delete
	find . -type f -name '*.ali' -delete

clean: clean-ada-files
	rm -f $(TARGET)
