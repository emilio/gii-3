IMAGES := $(wildcard src/img/*)
IMAGES := $(patsubst src/%, dist/%, $(IMAGES))

STYLESHEETS := $(wildcard src/*.sass)
STYLESHEETS := $(patsubst src/%.sass, dist/%.css, $(STYLESHEETS))

all: dist/profile.html $(STYLESHEETS) $(IMAGES)
	@echo > /dev/null

dist/%.css: src/%.sass
	@mkdir -p $(dir $@)
	sass $< $@

dist/%: src/%
	@mkdir -p $(dir $@)
	cp $< $@
