#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "mem_comp.h"
#include <string.h>

key_t shmkey = 0x200;

s_lote* attachMemoryBlock() {
    s_lote *lote;
    int id_bloque;
    //creacion de bloque
    if((id_bloque = shmget(shmkey, sizeof(s_lote), 0644 |IPC_CREAT)) == -1){ 
        perror("Error al crear bloque.");
        exit(1);
    }

    //mapear el bloque compartido en la memoria del proceso y devolver un apuntador

    lote = shmat(id_bloque, NULL, 0);
    if (lote == (s_lote*)-1) {
        perror("Error al mapear bloque.");
        exit(1);
    }

    lote->num_p = 0;

    return lote;
}

void detachMemoryBlock(int *block) {
    shmdt(block);
    printf("Block detached.\n");
}

void destroyMemoryBlock() {
    int id_bloque = shmget(shmkey, sizeof(int), 0);

    if (id_bloque == -1) {
        perror("Error al obtener bloque.");
        exit(1);
    }

    if (shmctl(id_bloque, IPC_RMID, NULL) == -1) {
        perror("Error al eliminar bloque.");
        exit(1);
    }
}
