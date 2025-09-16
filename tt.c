#include <stdlib.h>
#include <string.h>

#include "tt.h"

void Intt(Ttable *tt){
    tt->size = TABLESIZE;
    tt->table = (Table*)malloc(sizeof(Table) * tt->size);
    memset(tt->table, 0, sizeof(Table) * tt->size);
    
}
void Rett(Ttable *tt){
    for (int i = 0; i < tt->size; i++){
        tt->table[i].move = -1;
    }
}
Key Gettablekey(unsigned long long hash, int size){
    return hash % size;
}