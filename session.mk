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

ifeq ($(PANDOC_FLAGS),)
PANDOC_FLAGS := --toc
endif

TARGETS := $(TEX_TARGETS:.tex=.pdf) $(MD_TARGETS:.md=.pdf)

.PHONY: session-notes
session-notes: $(TARGETS)

%.pdf: %.tex
	$(info [DOC] $< -> $@)
	@pandoc $(PANDOC_FLAGS) --from=latex --latex-engine=xelatex --to=latex $< -o $@

%.pdf: %.md
	$(info [DOC] $< -> $@)
	@pandoc $(PANDOC_FLAGS) --from=markdown_github --latex-engine=xelatex --to=latex $< -o $@

