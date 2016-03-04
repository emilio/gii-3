# This is a really simple makefile to allow converting my markdown notes to
# pdf.
#
# Note that `notes.pdf` is ignored from the tree explicitely in this folder's
# gitignore.
#
# As of today (15/09/2015), the ideal folder structure would be:
#
# * session-x/
#   |- notes.{md,tex}              # My notes taken in markdown/latex
#	  |- notes.pdf                   # Auto-generated pdf for the notes
#   |- theory.{pdf,ppt,...}        # The theory provided for the session
#   |- paperwork/programs/etc      # Whatever we have to provide

TEX_TARGETS := $(wildcard *.tex)
MD_TARGETS := $(wildcard *.md)
DOT_TARGETS := $(wildcard img/*.dot)
DOT_TARGETS := $(DOT_TARGETS:.dot=.dot.png) \
               $(DOT_TARGETS:.dot=.dot.ps) \
							 $(DOT_TARGETS:.dot=.dot.pdf)

TARGETS := $(TEX_TARGETS:.tex=.pdf) \
           $(MD_TARGETS:.md=.pdf)

ifeq ($(PANDOC_FLAGS),)
PANDOC_FLAGS := --toc --filter pandoc-crossref
endif

.PHONY: session-notes
session-notes: $(TARGETS)

%.pdf: %.tex $(DOT_TARGETS)
	$(info [DOC] $< -> $@)
	@pandoc $(PANDOC_FLAGS) --from=latex --latex-engine=xelatex --to=latex $< -o $@

%.pdf: %.md $(DOT_TARGETS)
	$(info [DOC] $< -> $@)
	@pandoc $(PANDOC_FLAGS) --from=markdown --latex-engine=xelatex --to=latex $< -o $@

img/%.dot.png: img/%.dot
	dot -Gdpi=300 -Tpng $< -o $@

img/%.dot.ps: img/%.dot
	dot -Tps $< -o $@

img/%.dot.pdf: img/%.dot.ps
	ps2pdf $< $@

.PHONY: clean-session-notes
clean-session-notes:
	$(RM) $(TARGETS) $(DOT_TARGETS)
