#ifndef _Int64Hash_H
#define _Int64Hash_H

typedef struct
{
  int nentry;
    u_int64_t hashsize;
    int shift;
  u_int64_t *key;
  int *value;
}
sInt64Hash;

int Int64Hash_RegisterValue(sInt64Hash *ih, u_int64_t key,int value);
int Int64Hash_QueryValue(sInt64Hash *ih, u_int64_t key);
void Int64Hash_EraseOne(sInt64Hash *ih, u_int64_t key);
sInt64Hash *Int64Hash_Init(int size);
void Int64Hash_Done(sInt64Hash *ih);

#endif
