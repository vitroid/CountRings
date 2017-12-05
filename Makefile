all:
	echo Hello.
%: temp_%
	countrings -h | python3 util/replace.py %%usage%% "    " $< > $@
%.rst: %.md
	md2rst $<

install:
	make README.rst
	./setup.py install
pypi:
	make README.rst
	./setup.py check
	./setup.py sdist bdist_wheel upload
distclean:
	-rm -rf build dist
	-rm README.rst
	-rm .DS_Store
	find . -name __pycache__ | xargs rm -rf 
	find . -name \*.pyc      | xargs rm -rf
	find . -name \*~         | xargs rm -rf
