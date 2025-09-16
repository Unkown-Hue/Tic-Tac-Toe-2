#ifndef TT_H
#define TT_H

#include <stdint.h>

#include "game.h"

#define TABLESIZE 1000000

typedef unsigned long long Key;

typedef struct {
    uint8_t move;
    uint16_t score;
} Table;

typedef struct {
    Table *table;
    int size;
} Ttable;

void Intt(Ttable *tt);

void Rett(Ttable *tt);

Key Gettablekey(unsigned long long hash, int size);


#endif // TT_H