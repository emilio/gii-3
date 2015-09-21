# This is a really simple makefile to allow converting my markdown notes to
# pdf.
#
# Note that `notes.pdf` is ignored from the tree explicitely in this folder's
# gitignore.
#
# As of today (15/09/2015), the ideal folder structure would be:
#
# * session-x/
#   |- notes.md                    # My notes taken in markdown
#	  |- notes.pdf                   # Auto-generated pdf for the notes
#   |- theory.{pdf,ppt,...}        # The theory provided for the session
#   |- paperwork/programs/etc      # Whatever we have to provide

notes.pdf: notes.md
	$(info [DOC] $< -> $@)
	@pandoc --from=markdown_github --latex-engine=xelatex --to=latex $< -o $@
