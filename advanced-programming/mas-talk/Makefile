.PHONY: all
all: contents.pdf
	@echo > /dev/null

.PHONY: slides
slides: contents.md
	pandoc --self-contained -s -t revealjs $< -o $@

.PHONY: clean
clean:
	rm -f contents.pdf

PANDOC_FLAGS := --toc --filter pandoc-crossref

%.pdf: %.md
	$(info [DOC] $< -> $@)
	@pandoc $(PANDOC_FLAGS) --from=markdown --latex-engine=xelatex --to=latex $< -o $@
