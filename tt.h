#ifndef TT_H
#define TT_H

#include <stdint.h>

#include "game.h"

#define TABLESIZE 4000000

typedef unsigned long long Key;

typedef struct {
    uint16_t score;
    uint64_t hash;
    uint8_t depth;
} Table;

typedef struct {
    Table *table;
    int size;
    unsigned long long hit;
} Ttable;

void Intt(Ttable *tt, unsigned long long size);

void Rett(Ttable *tt);

Key Gettablekey(unsigned long long hash, int size);


#endif // TT_H
