#ifndef _BOND_H
#define _BOND_H

#define MAXBOND 12
typedef struct {
    int n;
    int to[MAXBOND];
    char flag[MAXBOND]; /* このプログラムでは使わない。 */
} BondType;

#endif
