CXXFLAGS=-I/usr/local/include/eigen3 -I/usr/local/Cellar/eigen/3.3.5/include/eigen3 -O -g

%.o: %.cpp rings.hpp
	$(CXX) $(CXXFLAGS) -c $<
TARGETS=test_rings
all: $(TARGETS)

test_rings: test_rings.o rings.o
	$(CXX) $(CXXFLAGS) $^ -o $@

test: test_rings
	./test_rings < ../samples/test.ngph
test2: test_rings
	./test_rings < /Volumes/workarea/work/bornAFP2/sfAFP.200K.ngph

clean:
	-rm *~ *.o $(TARGETS)
