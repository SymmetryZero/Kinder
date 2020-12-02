#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include "semaforos.h"
#include "mem_comp.h"
#include <errno.h>

key_t semkey = 0x100;

int main(int argc, char *argv[])
{
    int semaforos;
    int aux_index, aux_tiempo, aux_tiempo2, aux_prior, aux_prior2;
    char aux_nombre[30], aux_nombre2[30];
    s_lote *lote = attachMemoryBlock();

    if ((semaforos = iniciarSemaforo(semkey)) < 0) {
        perror("No se pudo crear semáforos.");
        exit(1);
    }

    while (1) {
        if (manejarSemaforo(semaforos, procesos_mutex, down) != 0) {
            printf("Semáforos eliminados. Ejecución terminada.\n");
            break;
        } 
        manejarSemaforo(semaforos, mutex, down);
        sleep(1);
        printf("Entrando en región crítica. Aceptando lote de procesos...\n\n");
        sleep(1);

        while (1) {
            aux_index = 0;
            aux_prior = -1;

            for (int i = 0; i < 20; ++i) {
                if (sscanf(lote->procesos[i], "%s %d %d", aux_nombre2, &aux_tiempo2, &aux_prior2) == 3) {
                    if (aux_prior2 > aux_prior) {
                        strcpy(aux_nombre, aux_nombre2);
                        aux_tiempo = aux_tiempo2;
                        aux_prior = aux_prior2;
                        aux_index = i;
                    }
                }
            }
            strcpy(lote->procesos[aux_index], "");
            if (aux_prior == -1) {
                printf("\n");
                break;
            }
            fflush(stdout);
            printf("Despachando proceso: %s,\t tiempo: %d,\t prioridad: %d\n", aux_nombre, aux_tiempo, aux_prior);
            nanosleep((struct timespec[]){{0, aux_tiempo * 20000000L}}, NULL);
        }

        lote->num_p = 0;

        sleep(1);

        printf("Saliendo de región crítica...\n\n");

        manejarSemaforo(semaforos, mutex, up);
    }

    detachMemoryBlock();
    destroyMemoryBlock(); //Destruye la memoria compartida

   
    return 0;
}
