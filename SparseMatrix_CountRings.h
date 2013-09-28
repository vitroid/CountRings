#ifndef _SPARSEMATRIX_COUNTRINGS_H
#define _SPARSEMATRIX_COUNTRINGS_H
#include "SparseMatrix.h"
#include "CountRings.h"

/*in SparseMatrix_CountRings.c*/
int CountRings2(RingType *ring,int max,BondType *bond,int bond_n,sSparseMatrix *path,int maxsize);
int SetBonds(sSparseMatrix *path,BondType *bond);

#endif
