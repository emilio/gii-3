
all: report.pdf
	@echo > /dev/null

%.pdf: %.md
	pandoc $< -f markdown -t latex -o $@ --filter pandoc-citeproc
