#include <stdio.h>
#include <stdlib.h>
#include "CountRings.h"

int tmpmain(void) {
    BondType *bond;
    RingType *ring;
    int **path;
    int bond_n;
    int ring_n;
    int count[MAXRINGSIZE];
    int ndata;
    int i,j;

    if ((bond = (BondType *) malloc(sizeof(BondType)*NMAX)) == NULL)
        HeapError();
    if ((ring = (RingType *) malloc(sizeof(RingType)*RINGMAX)) == NULL)
        HeapError();
    if ((path = (int **) malloc(sizeof(int *)*NMAX)) == NULL) HeapError();
    for (i = 0; i < NMAX; i++)
        if ((path[i] = (int *) malloc(sizeof(int)*NMAX)) == NULL)
            HeapError();

    for (ndata = 0; (bond_n = ReadBonds(bond,NMAX,stdin)) >= 0; ndata++) {
        if (!CheckBonds(bond,bond_n))
            {fprintf(stderr,"Data error!\n"); exit(1);}

        /* 員環構造の探索(３点固定、重複あり) */
        ring_n = CountRings(ring,RINGMAX,bond,bond_n,path,MAXRINGSIZE-1);
        //fprintf(stderr,"ring_n=%d  \n",ring_n);

        /* 重複の除去 */
        ring_n = SimplifyRings(ring,ring_n);
        //fprintf(stderr,"ring_n=%d  \n",ring_n);

        /* 員環数ごとのカウント */
        for (i = 0; i < MAXRINGSIZE; i++)
            count[i] = 0;
        for (i = 0; i < ring_n; i++)
            count[ring[i].n-1]++;

        /* 出力 */
        for (i = 3; i < MAXRINGSIZE; i++)
            printf(" %4d",count[i-1]);
        printf("\n");
    }

    /* 終了 */
    for (i = 0; i < NMAX; i++)
        free(path[i]);
    free(path);
    free(ring);
    free(bond);

    return 0;
}



int BondType_CountRings( int bond_n, BondType* bond, int maxRingSize, int* count )
{
    RingType *ring;
    int **path;
    int ring_n;
    int ndata;
    int i,j;

    if ((ring = (RingType *) malloc(sizeof(RingType)*RINGMAX)) == NULL)
        HeapError();
    if ((path = (int **) malloc(sizeof(int *)*NMAX)) == NULL) HeapError();
    for (i = 0; i < NMAX; i++)
        if ((path[i] = (int *) malloc(sizeof(int)*NMAX)) == NULL)
            HeapError();

    if (!CheckBonds(bond,bond_n)) {
        fprintf(stderr,"Data error!\n"); exit(1);
    }

    /* 員環構造の探索(３点固定、重複あり) */
    ring_n = CountRings(ring,RINGMAX,bond,bond_n,path, maxRingSize );
    //fprintf(stderr,"ring_n=%d  \n",ring_n);
    
    /* 重複の除去 */
    ring_n = SimplifyRings(ring,ring_n);
    //fprintf(stderr,"ring_n=%d  \n",ring_n);

    /* 員環数ごとのカウント */
    for (i = 0; i <= maxRingSize; i++)
        count[i] = 0;
    for (i = 0; i < ring_n; i++){
        //printf( "%d:", ring[i].n );
        count[ ring[i].n ]++;
    }
    //printf( "\n" );

    /* 終了 */
    for (i = 0; i < NMAX; i++)
        free(path[i]);
    free(path);
    free(ring);
}




/* 同一構造を除去 */
int SimplifyRings(RingType *ring,int ring_n) {
    int i,j,n;

    if (ring_n <= 0) return ring_n;
    qsort(ring,ring_n,sizeof(RingType),RingCompare);
    n = 0;
    for (i = 1; i < ring_n; i++) {
        if (RingCompare(ring+i,ring+n) != 0) {
            n++;
            ring[n].n = ring[i].n;
            for (j = 0; j < ring[i].n; j++)
                ring[n].list[j] = ring[i].list[j];
        }
    }

    return n+1;
}

/* 員環情報を比較する(qsort()用関数) */
int RingCompare(const void *e1,const void *e2) {
    RingType *r1,*r2;
    int i,ret;

    r1 = (RingType *) e1;
    r2 = (RingType *) e2;
    for (i = 0; i < r1->n && i < r2->n && r1->list[i] == r2->list[i]; i++)
       ;
    if (i < r1->n) {
        if (i < r2->n)
            ret = (r1->list[i] >= r2->list[i])? 1: -1;
        else
            ret = -1;
    } else {
        if (i < r2->n)
            ret = 1;
        else
            ret = 0;
    }
    return ret;
}

/* ｎ員環の数え上げ(返値は員環の総数、異常は負を返す。) */
int _crbuf_p[MAXRINGSIZE]; /* 粒子番号表 */
int _crbuf_b[MAXRINGSIZE]; /* 結合番号表 */
int _CountRings(RingType *ring,int max,BondType *bond,int **path, int maxRingSize ) {
    int ring_n = 0;
    int n,k,flag,*p;
    int i,j;

    flag = 1;
    for (n = 4; n <= maxRingSize && ring_n <= 0 && flag; n++) {
        /* n員環の探索 */
        _crbuf_b[2] = 0;
        i = 3;
        flag = 0;
        while (i >= 3) {
            if (i >= n) {
                flag = 1;
                if (path[_crbuf_p[0]][_crbuf_p[n-1]] == 1) {
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
                    p = path[_crbuf_p[i]];
                    for (j = 0; j < i-1
                                   && p[_crbuf_p[j]] >= _crmin(i-j,n-i+j); j++)
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
int CountRings(RingType *ring,int max,BondType *bond,int bond_n,int **path, int maxRingSize ) {
    int p0,p1,p2,b0,b1,b2;
    int ring_n = 0;
    int ret;
    int i,j;

    /* 各粒子間の最小距離を求める */
    MinPath(path,bond,bond_n);

    /* メインループ */
    for (p0 = 0; p0 < bond_n; p0++) {
        for (b0 = 0; b0 < bond[p0].n; b0++) {
            p1 = bond[p0].to[b0];
            for (b1 = 0; b1 < bond[p1].n; b1++) {
                p2 = bond[p1].to[b1];
                if (p0 >= p2) continue;
                if (path[p0][p2] == 1) {
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
                    ret = _CountRings(ring+ring_n,max-ring_n,bond,path, maxRingSize );
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

/* path[i][j]にi〜jへの最短路の距離を求める */
void MinPath(int **path,BondType *bond,int bond_n) {
    int work[NMAX];
    int head,tail;
    int *p;
    int i,j,k,to,length;

    /* 初期化 */
    for (i = 0; i < bond_n; i++)
        for (j = 0; j < bond_n; j++)
            path[i][j] = bond_n; /* 未計算の意味 */

    for (i = 0; i < bond_n; i++) {
        p = path[i];
        head = 0;
        tail = 1;
        p[i] = 0;
        work[0] = i;
        while (tail > head && tail < bond_n) {
            k = work[head++];
            length = p[k]+1;
            for (j = 0; j < bond[k].n; j++) {
                to = bond[k].to[j];
                if (p[to] >= bond_n) {
                    /* 新規登録 */
                    work[tail++] = to;
                }
                if (p[to] > length)
                    p[to] = length;
            }
        }
    }
}

/*  挿入ソート  */
void _insertsort(int n,int *a) {
    int i,j;
    int x;

    for (i = 1; i < n; i++) {
        x = a[i];
        for (j = i-1; j >= 0 && a[j] > x; j--)
            a[j+1] = a[j];
        a[j+1] = x;
    }
}

/* 有向グラフの読み込み(返値は粒子数、失敗は負。) */
int ReadBonds(BondType *bond,int max,FILE *fp) {
    char str[NMAX+1];
    int n;
    int i,j;

    /* 初期化 */
    for (i = 0; i < max; i++)
        bond[i].n = 0;

    /* 読み込み */
    n = max;
    for (i = 0; i < n && fgets(str,NMAX+1,fp) != NULL; i++) {
        for (j = 0; j < n && (str[j] == '0' || str[j] == '1'); j++) {
            if (j != i && str[j] == '1') {
                if (bond[i].n >= MAXBOND || bond[j].n >= MAXBOND)
                    {fprintf(stderr,"%d: Too many bonds.\n",i); return -1;}
                bond[i].to[bond[i].n++] = j;
                bond[j].to[bond[j].n++] = i;
            }
        }
        if (i == 0) n = j;
    }
    if (i < n || fgets(str,NMAX+1,fp) != NULL && *str != '\0' && *str != '\n')
        return -1;

    /* ソート */
    for (i = 0; i < n; i++)
        _insertsort(bond[i].n, bond[i].to);

    return n;
}

/* データの整合性判定 */
int CheckBonds(BondType *bond,int bond_n) {
    int i,j,k;
    int to;

    for (i = 0; i < bond_n; i++) {
        for (j = 0; j < bond[i].n; j++) {
	  to = bond[i].to[j];
	  if (to < 0 || to >= bond_n || (j > 0 && to <= bond[i].to[j-1])) {
	    fprintf(stderr,"%2d,%2d: Illegal bond %d found(1).\n",i,j,to);
	    return 0;
	  }
	  for (k = 0; k < bond[to].n && bond[to].to[k] != i; k++)
	    ;
	  if (k >= bond[to].n) {
	    fprintf(stderr,"%2d: Illegal bond %d found(2).\n",i,to);
	    return 0;
	  }
        }
    }
    return 1;
}

void HeapError(void) {
    fprintf(stderr,"Allocation failed.\n"); exit(1);
}


/*以上は馬場君(？)が作った算程群。基本的なアルゴリズムはこれでいいのだ
  が、巨大なシステムの場合、完全なpath[i][j]を作成するのは非常に浪費な
  ので、なんとか簡約したい。例えば、6員環までを知りたい場合、距離は3ま
  でで十分。任意に与えられた2点間の距離が即座に計算できれば、path[][]
  は必要ない。hash表で解決できるかも。

周期境界を横断するような環ができるケースがある。xy面、yz面、zx面を辺が
交差した回数をカウントしておけば、そのような環を(座標の情報なしで)除く
ことができるはず。*/


/*距離4以下の点をリストアップする。距離1の点対はあらかじめ
  RegisterDistanceで登録してあるものとする。無向グラフなら、i->jとj->i
  の両方が書かれていなければならない。*/


/*どうせこれはそのまま使うことはできないぞ。*/
