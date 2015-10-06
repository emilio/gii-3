TEX_FLAGS :=
TEX_REDIRECT := > /dev/null 2>&1

.PHONY: all
all: paperwork.pdf
	@echo > /dev/null

.PHONY: debug
debug: TEX_REDIRECT :=
debug: clean all

paperwork.pdf: paperwork.tex
	$(info [DOC] $< -> $@)
	@xelatex -shell-escape $(TEX_FLAGS) $< $(TEX_REDIRECT)
	@xelatex -shell-escape $(TEX_FLAGS) $< $(TEX_REDIRECT)
	@$(MAKE) --quiet clean-tex-secondary-files

.PHONY: clean-tex-secondary-files
clean-tex-secondary-files:
	rm -f paperwork.aux paperwork.log paperwork.nav paperwork.out paperwork.snm
	rm -f paperwork.lof paperwork.toc missfont.log
	rm -rf _minted*


.PHONY: clean
clean: clean-tex-secondary-files
	@rm -f paperwork.pdf

include ../../session.mk

