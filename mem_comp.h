#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <stdbool.h>
#include <sys/types.h>

typedef struct struct_lote {
    char procesos[20][30];
    int num_p;
} s_lote;

s_lote* attachMemoryBlock();
void detachMemoryBlock();
void destroyMemoryBlock();

#define BLOCK_SIZE sizeof(int)

#endif
