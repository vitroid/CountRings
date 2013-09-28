#ifndef _SPARSEMATRIX_H
#define _SPARSEMATRIX_H
#include "Int64Hash.h"

/* 2!3! = 12重にcountするはず。*/

typedef struct
{
  int nadj;
  int *adj;
}
sLineElement;

typedef struct
{
  /*size of the matrix*/
  int nline,ncolumn,maxadj;
  sLineElement *e_line,*e_column;
  sInt64Hash *ih;
}
sSparseMatrix;

void SparseMatrix_RegisterValue(sSparseMatrix *path,int i,int j,int value);
int SparseMatrix_QueryValue(sSparseMatrix *path,int i,int j);
sSparseMatrix *SparseMatrix_Init(int nline,int ncolumn,int hashbit,int maxadj);
void SparseMatrix_Done(sSparseMatrix *path);
sSparseMatrix *SparseMatrix_LoadSSMX(FILE *fp,int hashbit,int maxadj);
sSparseMatrix *SparseMatrix_LoadNGPH(FILE *fp,int hashbit,int maxadj);
sSparseMatrix *SparseMatrix_LoadAsymNGPH(FILE *fp,int hashbit,int maxadj);
sSparseMatrix *SparseMatrix_LoadGRPH(FILE *fp,int hashbit,int maxadj);
sSparseMatrix *SparseMatrix_LoadAsymGRPH(FILE *fp,int hashbit,int maxadj);
void SparseMatrix_SaveSMTX(FILE *fp,sSparseMatrix *path);
/*compatibility for older version*/
#define SparseMatrix_RegisterBasicElement(a,b,c,d) SparseMatrix_RegisterValue(a,b,c,d)



#endif
