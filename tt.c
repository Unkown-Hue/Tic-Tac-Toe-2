

#include "tt.h"

void Intt(Ttable *tt){
    tt->size = TABLESIZE;
    tt->table = (Table*)malloc(sizeof(Table) * tt->size);
    memset(tt->table, 0, sizeof(Table) * tt->size);
    
}
void Rett(Ttable *tt){
    memset(tt->table, 0, sizeof(Table) * tt->size);
}
inline Key Gettablekey(unsigned long long hash, int size){
    return hash % size;
}