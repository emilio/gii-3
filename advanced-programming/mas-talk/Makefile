
.PHONY: all
all: contents.pdf
	@echo > /dev/null

.PHONY: slides
slides: contents.md
	pandoc --self-contained -s -t revealjs $< -o $@

.PHONY: clean
clean:
	rm -f contents.pdf

include ../../session.mk
