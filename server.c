#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "semaforos.h"
#include "mem_comp.h"

key_t semkey = 0x100;


int main(int argc, char **argv){
    if(argc > 1){
        //Definicion de variables 
        int fd, fd2, puerto;
        puerto = atoi(argv[1]);

        int semaforos;
        char proceso[30];
        s_lote *lote = attachMemoryBlock();
        char lista[20][30];

        //Semáforos
        if ((semaforos = iniciarSemaforo(semkey)) < 0) {
            perror("No se pudo crear semáforos.");
            exit(1);
        }

        //Se crean dos estructuras de tipo sockaddr, la primera guarda info del server y la segunda del cliente
        struct sockaddr_in server;
        struct sockaddr_in client;

        //configurando el servidor
        server.sin_family = AF_INET;  //Familia TCP/IP
        server.sin_port = htons(puerto); //puerto
        server.sin_addr.s_addr = INADDR_ANY; //Cualquier cliente puede conectarse
        bzero(&(server.sin_zero), 8); //Funcion que rellena con 0's

        //segudo paso, definición de socket
        if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
            perror("Error de pertura de socket");
            exit(-1);
        }

        //tercer paso, avisar al sistema que se creo un socket
        if(bind(fd, (struct sockaddr*)&server, sizeof(struct sockaddr)) == -1){
            printf("error en bind() \n");
            exit(-1);
        }

        //Cuarto paso, establecer el socket en modo escucha
        if(listen(fd, 5) == -1){
            printf("error en listen()\n");
            exit(-1);
        }

        //Quinto paso, aceptar conexiones
        socklen_t longitud_cliente = sizeof(struct sockaddr_in);

        /* A continuación la llamada a accept() */
        if ((fd2 = accept(fd, (struct sockaddr *)&client, &longitud_cliente)) == -1){
            printf("error en accept()\n");
            exit(-1);
        }

        /* int i = 0; */
        int flag = 1;
        while(1) {
            manejarSemaforo(semaforos, mutex, down);
            fflush(stdout);
            sleep(1);
            printf("Entrando en región crítica. ");

            if (lote->num_p == 0) {
                for (int i = 0; i < 20; ++i) {
                    int length;

                    if ((flag = recv(fd2, &length, sizeof(length), 0)) == 0) {
                        detachMemoryBlock();
                        semctl(semaforos, 0, IPC_RMID); //Borra semáforos para terminar ejecución del desp
                        break; //Se rompe cuando el socket del cliente se cierra
                    }
                    recv(fd2, proceso, length, 0);
                    strcpy(lote->procesos[i], proceso);
                }
                lote->num_p = 20;
            }

            if (flag == 0) {
                sleep(1);
                printf("Buffer vacío. Ejecución terminada.\n");
                break;
            }

            printf("Enviando lote de procesos...\n\n");
            sleep(3);

            printf("Saliendo de región crítica...\n\n");
            manejarSemaforo(semaforos, procesos_mutex, up);
            manejarSemaforo(semaforos, mutex, up);

            sleep(1);
        }

        close(fd2);
        close(fd);
    } else {
        printf("No se ingresó puerto por parámetro. Uso: ./server <puerto>\n");
    }
    return 0;

}
