#include <stdlib.h>
#include <string.h>

#include "tt.h"

void Intt(Ttable *tt, unsigned long long size){
    tt->size = size;
    tt->table = (Table*)malloc(sizeof(Table) * tt->size);
    tt->hit = 0;
    for (int i = 0; i < tt->size; i++){
        tt->table[i].hash = 0;
    }
    
}
void Rett(Ttable *tt){
    tt->hit = 0;
    for (int i = 0; i < tt->size; i++){
        tt->table[i].hash = 0;
    }
}
Key Gettablekey(unsigned long long hash, int size){
    return hash % size;
}

Key Gettablehash(State *st, int size){
    return (st->hash ^ st->mhash) % size;
}