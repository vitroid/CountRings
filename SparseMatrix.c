/*
 * SparseMatrix.c
 * 
 * Treating sparse matrices with hash table
 * $Id: SparseMatrix.c,v 1.2 2007/09/20 09:45:39 matto Exp $
 * $Log: SparseMatrix.c,v $
 * Revision 1.2  2007/09/20 09:45:39  matto
 * Version down. varray is removed because of miscounting the 8 membered rings in CountRings/test.ngph
 *
 * Revision 1.2  2004-01-27 08:07:09  matto
 * IntHash is replaced by Int64Hash.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SparseMatrix.h"

/*要素(i,j)にvalueを登録する。valueが0の場合、本来ならHashからデータを消し、隣
  接情報からも除かなければいけないが、実際的な必要が生じていないので、
  ここではvalueが0のケースは考えていない。*/
void SparseMatrix_RegisterValue(sSparseMatrix *path,int i,int j,int value)
{
  int k;
  int new=Int64Hash_RegisterValue(path->ih,(u_int64_t)i*(u_int64_t)path->ncolumn+(u_int64_t)j,value);
  if(new){
    if(path->e_line[i].nadj>=path->maxadj-1){
      fprintf(stderr,"Too many elements in line %d\n",i);
      exit(1);
    }
    path->e_line[i].adj[path->e_line[i].nadj++]=j;
    if(path->e_column[j].nadj>=path->maxadj-1){
      fprintf(stderr,"Too many elements in column %d\n",i);
      exit(1);
    }
    path->e_column[j].adj[path->e_column[j].nadj++]=i;
  }
}


/*要素(i,j)の値を返す。*/
int SparseMatrix_QueryValue(sSparseMatrix *path,int i,int j)
{
  return Int64Hash_QueryValue(path->ih,(u_int64_t)i*(u_int64_t)path->ncolumn+(u_int64_t)j);
}

void _sp_hasherror()
{
  fprintf(stderr,"Hash error. Halted.\n");
  exit(1);
}


/*空疎対称行列の初期化*/
sSparseMatrix *SparseMatrix_Init(int nline,int ncolumn,int hashbit,int maxadj)
{
  int i;
  sSparseMatrix *path=malloc(sizeof(sSparseMatrix));
  path->nline=nline;
  path->ncolumn=ncolumn;
  path->maxadj=maxadj;
  path->ih=Int64Hash_Init(hashbit);
  if(NULL==(path->e_line=calloc(nline,sizeof(sLineElement))))
    _sp_hasherror();
  if(NULL==(path->e_column=calloc(ncolumn,sizeof(sLineElement))))
    _sp_hasherror();
  for(i=0;i<nline;i++){
    if(NULL==(path->e_line[i].adj=malloc(sizeof(int)*maxadj)))
      _sp_hasherror();
  }
  for(i=0;i<ncolumn;i++){
    if(NULL==(path->e_column[i].adj=malloc(sizeof(int)*maxadj)))
      _sp_hasherror();
  }
  return path;
}

/*空疎対称行列のdestructor*/
void SparseMatrix_Done(sSparseMatrix *path)
{
  int i;
  for(i=0;i<path->nline;i++){
    free(path->e_line[i].adj);
  }
  for(i=0;i<path->ncolumn;i++){
    free(path->e_column[i].adj);
  }
  free(path->e_line);
  free(path->e_column);
  Int64Hash_Done(path->ih);
  free(path);
}

/* 空疎対称行列の読み込み */
sSparseMatrix *SparseMatrix_LoadSSMX(FILE *fp,int hashbit,int maxadj) {
  int n;
  char buf[256];
  sSparseMatrix *path;
  /*SSMX形式を想定。1行目は節点の数*/
  fgets(buf,sizeof(buf),fp);
  n=atoi(buf);
  /*初期化*/
  path=SparseMatrix_Init(n,n,hashbit,maxadj);
  /*隣接関係の読みこみ。*/
  while(NULL!=fgets(buf,sizeof(buf),fp)){
    int i,j,v;
    sscanf(buf,"%d %d %d",&i,&j,&v);
    /**/
    if(i<0)break;
    /*無向グラフとしてあつかう。*/
    SparseMatrix_RegisterValue(path,i,j,v);
    if(i!=j)
      SparseMatrix_RegisterValue(path,j,i,v);
  }
  return path;
}

/* 二値空疎対称行列の読み込み */
sSparseMatrix *SparseMatrix_LoadNGPH(FILE *fp,int hashbit,int maxadj) {
  int n;
  char buf[256];
  sSparseMatrix *path;
  /*NGPH形式を想定。1行目は節点の数*/
  fgets(buf,sizeof(buf),fp);
  n=atoi(buf);
  /*初期化*/
  path=SparseMatrix_Init(n,n,hashbit,maxadj);
  /*隣接関係の読みこみ。*/
  while(NULL!=fgets(buf,sizeof(buf),fp)){
    int i,j,v;
    sscanf(buf,"%d %d",&i,&j);
    /**/
    if(i<0)break;
    /*無向グラフとしてあつかう。*/
    SparseMatrix_RegisterValue(path,i,j,1);
    if(i!=j)
      SparseMatrix_RegisterValue(path,j,i,1);
  }
  return path;
}

/* 二値空疎非対称行列の読み込み */
sSparseMatrix *SparseMatrix_LoadAsymNGPH(FILE *fp,int hashbit,int maxadj) {
  int n;
  char buf[256];
  sSparseMatrix *path;
  /*NGPH形式を想定。1行目は節点の数*/
  fgets(buf,sizeof(buf),fp);
  n=atoi(buf);
  /*初期化*/
  path=SparseMatrix_Init(n,n,hashbit,maxadj);
  /*隣接関係の読みこみ。*/
  while(NULL!=fgets(buf,sizeof(buf),fp)){
    int i,j,v;
    sscanf(buf,"%d %d",&i,&j);
    /**/
    if(i<0)break;
    /*無向グラフとしてあつかう。*/
    SparseMatrix_RegisterValue(path,i,j,1);
  }
  return path;
}

/* 二値対称行列の読み込み */
sSparseMatrix *SparseMatrix_LoadGRPH(FILE *fp,int hashbit,int maxadj) {
  int n,i,j;
  char buf[10000];
  sSparseMatrix *path;
  /*GRPH形式を想定。1行目は節点の数*/
  fgets(buf,sizeof(buf),fp);
  n=atoi(buf);
  /*初期化*/
  path=SparseMatrix_Init(n,n,hashbit,maxadj);
  /*隣接関係の読みこみ。*/
  for(i=0;i<n;i++){
    if(NULL==fgets(buf,sizeof(buf),fp)){
      SparseMatrix_Done(path);
      return NULL;
    }
    for(j=0;j<n;j++)
      if(buf[j]=='1'){
	SparseMatrix_RegisterValue(path,i,j,1);
	if(i!=j)
	  SparseMatrix_RegisterValue(path,j,i,1);
      }
  }
  return path;
}

/* 二値非対称行列の読み込み */
sSparseMatrix *SparseMatrix_LoadAsymGRPH(FILE *fp,int hashbit,int maxadj) {
  int n,i,j;
  char buf[10000];
  sSparseMatrix *path;
  /*GRPH形式を想定。1行目は節点の数*/
  fgets(buf,sizeof(buf),fp);
  n=atoi(buf);
  /*初期化*/
  path=SparseMatrix_Init(n,n,hashbit,maxadj);
  /*隣接関係の読みこみ。*/
  for(i=0;i<n;i++){
    if(NULL==fgets(buf,sizeof(buf),fp)){
      SparseMatrix_Done(path);
      return NULL;
    }
    for(j=0;j<n;j++)
      if(buf[j]=='1'){
	SparseMatrix_RegisterValue(path,i,j,1);
      }
  }
  return path;
}

/* 出力。(確認用) */
void SparseMatrix_SaveSMTX(FILE *fp,sSparseMatrix *path) {
  int n;
  char buf[256];
  int i,j;
  if(path->nline!=path->ncolumn){
    fprintf(stderr,"Warning: Not a square matrix.\n");
  }
  fprintf(fp,"@SMTX\n%d\n",path->nline);
  for(i=0;i<path->nline;i++){
    for(j=0;j<path->ncolumn;j++){
      fprintf(fp,"%d ",SparseMatrix_QueryValue(path,i,j));
    }
    fprintf(fp,"\n");
  }
}

void SparseMatrix_SaveNGPH(FILE *fp,sSparseMatrix *path) {
  int n;
  char buf[256];
  int i,j;
  if(path->nline!=path->ncolumn){
    fprintf(stderr,"Warning: Not a square matrix.\n");
  }
  fprintf(fp,"@NGPH\n%d\n",path->nline);
  for(i=0;i<path->nline;i++){
    for(j=0;j<path->e_line[i].nadj;j++){
      int k=path->e_line[i].adj[j];
      int v=SparseMatrix_QueryValue(path,i,k);
      if(v>1){
	fprintf(stderr,"Warning: not a binary value %d at element %d %d of NGPH.\n",v,i,k);
      }
      if(v){
	fprintf(fp,"%d %d\n",i,k);
      }
    }
  }
  fprintf(fp,"-1 -1\n");
}
