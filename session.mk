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

.PHONY: session-notes
session-notes: notes.pdf

%.pdf: %.tex
	$(info [DOC] $< -> $@)
	@pandoc --from=latex --latex-engine=xelatex --to=latex $< -o $@

%.pdf: %.md
	$(info [DOC] $< -> $@)
	@pandoc --from=markdown_github --latex-engine=xelatex --to=latex $< -o $@

