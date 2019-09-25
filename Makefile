PKGNAME=CountRings
all:
	echo Hello.
%: temp_%
	countrings -h | python3 util/replace.py %%usage%% "    " $< > $@

test-deploy: build
	twine upload -r pypitest dist/*
test-install:
	pip3 install --index-url https://test.pypi.org/simple/ $(PKGNAME)

install: check
	./setup.py install
uninstall:
	pip uninstall $(PKGNAME)
build: README.md
	./setup.py sdist bdist_wheel
deploy: build
	twine upload dist/*
check:
	./setup.py check
distclean:
	-rm -rf build dist
	-rm README.rst
	-rm .DS_Store
	find . -name __pycache__ | xargs rm -rf 
	find . -name \*.pyc      | xargs rm -rf
	find . -name \*~         | xargs rm -rf
