/*整数(0を含まない)を要素とするHash。*/

#include <stdio.h>
#include <stdlib.h>
#include "Int64Hash.h"
#define EMPTY -1

/*shiftとxorによる簡単なhash keyの生成*/
u_int64_t _Int64Hash_Encode(sInt64Hash *s, u_int64_t key)
{
    u_int64_t mod=0;
    while(key)
      {
          mod ^= (key & (s->hashsize-1));
          key >>= s->shift;
      }
    return mod;
}

/*hash要素の番号を返す。もし存在しない要素なら、空き要素を返す。要素は追加されない。*/
u_int64_t _Int64Hash_QueryElement(sInt64Hash *ih, u_int64_t key)
{
  u_int64_t e=_Int64Hash_Encode(ih,key);
  while(1){
    if(ih->key[e]==EMPTY){
      ih->value[e]=0;
      return e;
    }
    if(ih->key[e]==key){
      return e;
    }
    e+=13;
    if(e>=ih->hashsize)
      e-=ih->hashsize;
  }
}

/*値を登録する。更新なら0、追加なら1を返す。*/
int Int64Hash_RegisterValue(sInt64Hash *ih, u_int64_t key,int value)
{
    u_int64_t e=_Int64Hash_QueryElement(ih,key);
    int v=ih->value[e];
    if(v==0){
        ih->key[e]=key;
        ih->nentry++;
        if(ih->nentry > ih->hashsize/2){
            fprintf(stderr,"Warning: hash size seems too small.\n");
        }
        if(ih->nentry >= ih->hashsize){
            fprintf(stderr,"Error: hash overflow.\n");
            exit(1);
        }
    }
    ih->value[e]=value;
    return (v==0);
}

/*値を参照する。*/
int Int64Hash_QueryValue(sInt64Hash *ih,u_int64_t key)
{
  return ih->value[_Int64Hash_QueryElement(ih,key)];
}

/*値を抹消する。*/
void Int64Hash_EraseOne(sInt64Hash *ih,u_int64_t key)
{
  ih->key[_Int64Hash_QueryElement(ih,key)]=EMPTY;
}

sInt64Hash *Int64Hash_Init(int size)
{
    sInt64Hash *ih=malloc(sizeof(sInt64Hash));
    int i;
    u_int64_t m;
    
    m=1<<size;
    ih->shift=size;
    ih->hashsize=m;
    ih->key=malloc(m*sizeof(u_int64_t));
    ih->value=calloc(m,sizeof(int));
    ih->nentry=0;
    for(i=0;i<m;i++)
        ih->key[i]=EMPTY;
    return ih;
}

void Int64Hash_Done(sInt64Hash *ih)
{
    free(ih->key);
    free(ih->value);
    free(ih);
}
