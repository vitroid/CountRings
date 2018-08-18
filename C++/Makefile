CXXFLAGS=-I/usr/local/Cellar/eigen/3.3.5/include/eigen3

%.o: %.cpp rings.hpp dijkstra2_eigen.hpp
	$(CXX) $(CXXFLAGS) -c $<
TARGETS=test_rings test_dijkstra2_eigen
all: $(TARGETS)

test_rings: test_rings.o rings.o dijkstra2_eigen.o
	$(CXX) $(CXXFLAGS) $^ -o $@
test_dijkstra2_eigen: test_dijkstra2_eigen.o dijkstra2_eigen.o
	$(CXX) $(CXXFLAGS) $^ -o $@

test: test_rings
	./test_rings < ../samples/test.ngph

clean:
	-rm *~ *.o $(TARGETS)
