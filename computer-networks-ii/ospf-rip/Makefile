TEX_TARGETS := $(wildcard *.tex)
TARGETS := $(TEX_TARGETS:.tex=.pdf)

.PHONY: all
all: $(TARGETS)
	@echo > /dev/null

%.pdf: %.tex
	$(info [DOC] $< -> $@)
	@xelatex $<
	@xelatex $<
