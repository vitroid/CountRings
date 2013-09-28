#include <stdio.h>
#include <stdlib.h>
#include "SparseMatrix.h"
#include "CountRings.h"

extern int _crbuf_p[MAXRINGSIZE]; /* 粒子番号表 */
extern int _crbuf_b[MAXRINGSIZE]; /* 結合番号表 */
void MinPath2(sSparseMatrix *path,BondType *bond,int bond_n,int maxpath) {
    int work[NMAX];
    int head,tail;
    int i,j,k,to,length;
  
  /* 初期化 */
  /*for (i = 0; i < bond_n; i++)
    for (j = 0; j < bond_n; j++)
    path[i][j] = bond_n; /* 未計算の意味 */
  
  for (i = 0; i < bond_n; i++) {
      //fprintf(stderr,"%d\r",i);
    for(j=0;j<bond[i].n;j++){
      work[j]=bond[i].to[j];
    }
    head = 0;
    tail = j;
    while (tail > head) {
      k = work[head++];
      length = SparseMatrix_QueryValue(path,i,k)+1;
      /*printf("---[%d %d]=%d(%d %d)\n",i,k,length,bond[0].n,tail);*/
      for (j = 0; j < bond[k].n; j++) {
	int value;
	to = bond[k].to[j];
	if(i!=to){
	  value=SparseMatrix_QueryValue(path,i,to);
	  /*printf("(%d %d)=%d\n",i,to,value);*/
	  if (value==0 && length <= maxpath) {
              /* 新規登録 */
              work[tail++] = to;
	    SparseMatrix_RegisterValue(path,i,to,length);
	  }else if (value > length)
	    SparseMatrix_RegisterValue(path,i,to,length);
	}
      }
      
    }
  }
}

int _CountRings2(RingType *ring,int max,BondType *bond,sSparseMatrix *path) {
    int ring_n = 0;
    int n,k,flag;
    int i,j;

    flag = 1;
    for (n = 4; n <= MAXRINGSIZE && ring_n <= 0 && flag; n++) {
      
        /* n員環の探索 */
        _crbuf_b[2] = 0;
        i = 3;
        flag = 0;
        while (i >= 3) {
            if (i >= n) {
                flag = 1;
                if (SparseMatrix_QueryValue(path,_crbuf_p[0],_crbuf_p[n-1]) == 1) {
                    /* n員環を発見、登録 */
                    if (ring_n < max) {
                        ring[ring_n].n = n;
                        k = 0;
                        for (j = 1; j < n; j++)
                            if (_crbuf_p[j] < _crbuf_p[k])
                                k = j;
                        if (_crbuf_p[(k-1+n)%n] > _crbuf_p[(k+1)%n]) {
                            for (j = 0; j < n; j++)
                                ring[ring_n].list[j] = _crbuf_p[(k+j)%n];
                        } else {
                            for (j = 0; j < n; j++)
                                ring[ring_n].list[j] = _crbuf_p[(k-j+n)%n];
                        }
                        ring_n++;
                    } else
                        return -1; /* 登録不能、異常終了 */
                }
            } else {
                if (_crbuf_b[i-1] < bond[_crbuf_p[i-1]].n) {
                    _crbuf_p[i] = bond[_crbuf_p[i-1]].to[_crbuf_b[i-1]];
                    /* 粒子間距離の判定 */
                    /*p = path[_crbuf_p[i]];*/
                    for (j = 0; j < i-1
                                   && SparseMatrix_QueryValue(path,_crbuf_p[i],_crbuf_p[j]) >= _crmin(i-j,n-i+j); j++)
                        ;
                    if (j >= i-1) {
                        /* 採択。１つ下がる */
                        _crbuf_b[i++] = 0;
                    } else {
                        /* 棄却。次を試す */
                        _crbuf_b[i-1]++;
                    }
                    continue;
                }
            }
            /* １つ上がる */
            i--;
            _crbuf_b[i-1]++; /* 最後に_crbuf_b[1]++をしてしまうが問題なし */
        }
    }

    return ring_n;
}

/* 全員環数の数え上げ(返値は員環の総数、異常は負を返す。) */
int CountRings2(RingType *ring,int max,BondType *bond,int bond_n,sSparseMatrix *path,int maxsize) {
    int p0,p1,p2,b0,b1,b2;
    int ring_n = 0;
    int ret;
    int i,j;

    /* 各粒子間の最小距離を求める */
    MinPath2(path,bond,bond_n,maxsize/2);
    /*for(i=0;i<bond_n;i++){
      for(j=0;j<bond_n;j++){
	printf("%d ",SparseMatrix_QueryValue(path,i,j));
      }
      printf("\n");
    }
    exit(1);
    */
    /* メインループ */
    for (p0 = 0; p0 < bond_n; p0++) {
        for (b0 = 0; b0 < bond[p0].n; b0++) {
            p1 = bond[p0].to[b0];
            for (b1 = 0; b1 < bond[p1].n; b1++) {
                p2 = bond[p1].to[b1];
                if (p0 >= p2) continue;
                if (SparseMatrix_QueryValue(path,p0,p2) == 1) {
                    /* ３員環発見、登録 */
                    if (p0 < p1 && p1 < p2) {
                        if (ring_n < max) {
                            ring[ring_n].n = 3;
                            ring[ring_n].list[0] = p0;
                            ring[ring_n].list[1] = p1;
                            ring[ring_n].list[2] = p2;
                            ring_n++;
                        } else
                            return -1; /* 登録不能、異常終了 */
                    }
                } else {
                    /* ｎ員環探索(ｎ＞３) */
                    _crbuf_p[0] = p0;
                    _crbuf_p[1] = p1;
                    _crbuf_p[2] = p2;
                    _crbuf_b[0] = b0;
                    _crbuf_b[1] = b1;
                    _crbuf_b[2] = 0;
                    ret = _CountRings2(ring+ring_n,max-ring_n,bond,path);
                    if (ret >= 0)
                        ring_n += ret;
                    else
                        return -1; /* 登録不能、異常終了 */
                }
            }
        }
    }

    return ring_n;
}

int SetBonds(sSparseMatrix *path,BondType *bond) {
    int i,j;

    /* 初期化 */

    for (i = 0; i < path->nline; i++)
        bond[i].n = 0;

    /* 読み込み */
    for(i=0;i<path->nline;i++){
      int n=bond[i].n=path->e_line[i].nadj;
      for(j=0;j<n;j++){
	bond[i].to[j]=path->e_line[i].adj[j];
      }
    }

    /* ソート */
    for (i = 0; i < path->nline; i++)
      _insertsort(bond[i].n, bond[i].to);

    return path->nline;
}

