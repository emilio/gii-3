
paperwork.pdf: paperwork.tex
	$(info [DOC] $< -> $@)
	@xelatex -shell-escape $< > /dev/null 2>&1
	@rm -f paperwork.aux
	@rm -f paperwork.log

include ../../session.mk

