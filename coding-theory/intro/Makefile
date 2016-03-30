MD_TARGETS := $(wildcard *.md)
DOT_TARGETS := $(wildcard img/*.dot)
DOT_TARGETS := $(DOT_TARGETS:.dot=.dot.png) \
               $(DOT_TARGETS:.dot=.dot.ps) \
							 $(DOT_TARGETS:.dot=.dot.pdf)

TARGETS := $(MD_TARGETS:.md=.pdf) \
           $(MD_TARGETS:.md=.tex) \
           $(DOT_TARGETS)

PANDOC_FLAGS := --toc --filter pandoc-citeproc

.PHONY: session-notes
all: $(TARGETS)

%.tex: %.md
	$(info [DOC] $< -> $@)
	@pandoc $(PANDOC_FLAGS) -s --from=markdown --latex-engine=xelatex --to=latex $< -o $@

%.pdf: %.md $(DOT_TARGETS)
	$(info [DOC] $< -> $@)
	@pandoc $(PANDOC_FLAGS) -s --from=markdown --latex-engine=xelatex --to=latex $< -o $@

img/%.dot.png: img/%.dot
	dot -Gdpi=300 -Tpng $< -o $@

img/%.dot.ps: img/%.dot
	dot -Tps $< -o $@

img/%.dot.pdf: img/%.dot.ps
	ps2pdf $< $@

.PHONY: clean-session-notes
clean:
	$(RM) $(TARGETS) $(DOT_TARGETS)
