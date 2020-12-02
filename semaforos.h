#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>


#define up 1
#define down -1

#define procesos_mutex 0 //mutex para disponibilidad de procesos
#define mutex 1 //mutex para disponibilidad de sillas

#define sillas 10 //Total de sillas
#define SEMPERM 0600

int iniciarSemaforo(key_t semkey);
int manejarSemaforo(int semid, int numsem, int op);

