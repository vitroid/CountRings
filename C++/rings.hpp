#ifndef _RINGS_HPP
#define _RINGS_HPP

#include "dijkstra2_eigen.hpp"


using namespace std;
typedef vector<int> sRing;
typedef vector<sRing> sRings;

#define MAXRINGSIZE 12

void
showring(const sRing& ring);

void
showringf(const sRing& ring);

void
showrings(const sRings& rings);

void
showringsf(const sRings& rings);

sRings
countrings(sMatrix& m, const sMatrix& dm, int maxringsize);


#endif
