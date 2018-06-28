all:
	ls *.ngph | sed -e s/ngph/rngs/ | xargs make

%.rngs: %.ngph
	../countrings.x < $< > $@
