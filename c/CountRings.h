#ifndef _COUNTRINGS_H
#define _COUNTRINGS_H
#include "Bond.h"

void HeapError(void);

#define MAXRINGSIZE 11
typedef struct {
    int n;
    int list[MAXRINGSIZE];
} RingType;

int  ReadBonds(BondType *bond,int max,FILE *fp);
int  CheckBonds(BondType *bond,int bond_n);
void MinPath(int **path,BondType *bond,int bond_n);
int  CountRings(RingType *ring,int max,BondType *bond,int bond_n,int **path, int maxRingSize );
int  SimplifyRings(RingType *ring,int ring_n);
int  RingCompare(const void *e1,const void *e2);

#define NMAX 4097
#define RINGMAX 180000

#define _crmin(x,y) (((x)<(y))?(x):(y))

void _insertsort(int n,int *a);

#endif
