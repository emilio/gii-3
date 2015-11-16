
%.o: %.adb
	gnatmake $<

%: %.adb
	gnatmake $<

clean-ada-files:
	find . -type f -name '*.o' -delete
	find . -type f -name '*.ali' -delete
