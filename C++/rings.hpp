#ifndef _RINGS_HPP
#define _RINGS_HPP

#include <iostream>
#include <vector>
#include <Eigen/Sparse>


using namespace std;
using namespace Eigen; 

typedef vector<int> sRing;
typedef vector<sRing> sRings;
typedef SparseMatrix<int> sMatrix; 

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
sMatrix
DistanceMatrix(const sMatrix& m,int maxpath);
void
showmat(const sMatrix& m);


#endif
