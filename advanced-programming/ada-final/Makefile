.PHONY: all
all: final
	@echo > /dev/null

final: nuclear_central.o

.PHONY: test
test:
	timeout 15 ./final > test.txt || true
	[ $$(cat test.txt | grep 'ESTABLE\|PELIGRO' | wc -l) -eq 50 ] || (echo "Failure!" && false)
	@echo "Ok!"

%.o: %.adb %.ads
	gnatmake -gnatwa $<

%.o: %.adb
	gnatmake -gnatwa $<

%: %.adb
	gnatmake -gnatwa $<

clean-ada-files:
	find . -type f -name '*.o' -delete
	find . -type f -name '*.ali' -delete

clean: clean-ada-files
	rm -f final
