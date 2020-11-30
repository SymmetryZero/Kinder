#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <netdb.h>
#include <netinet/in.h>
#include "generarArchivoAleatorio.h"

int main(int argc, char *argv[]){
    if(argc > 1){
        // Definicion de variables
        char *ip;
        int fd, numbytes, puerto, cursor = 0;
        char buf[100];
        puerto = atoi(argv[2]);
        ip = argv[1];
        FILE *f_procesos;

        /* int tiempo, prioridad; */
        char proceso[30];

        generarArchivoDeProcesosPorLotes();

        struct hostent *he;
        /* estructura que recibe informacion sobre el nodo remoto */
        struct sockaddr_in server;
        // informacion sobre la direccion del servidor

        if((he = gethostbyname(ip)) == NULL){
            // llamada a gethostbynane ()
            printf("gethostbyname() error\n");
            exit(-1);
        }

        // Paso 2, definicion de socket
        if((fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
            // llamada a socket()
            printf("socket() error/n");
            exit(-1);
        }
        // Datos del servidor
        server.sin_family = AF_INET;
        server.sin_port = htons(puerto);
        server.sin_addr = *((struct in_addr *) he->h_addr);
        // *he->h_addr pasa la informacion de *he a *h_addr
        bzero(&(server.sin_zero),8);

        // paso 3, conectamos al servidor
        /* if((numbytes = recv(fd, buf, 100, 0)) == 1){ */
        /*     // llamada a recv() */
        /*     printf("error en recv()\n"); */
        /*     exit(-1); */

        /* } */
        f_procesos = fopen("procesos.txt", "r");

        for (int i = 0; i < 25; ++i) {
            for (int j = 0; j < 20; ++j) {
                /* fscanf(f_procesos, "%s", proceso); */
                if(connect(fd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == 1){
                    // llamada a connect()
                    printf("Conect() error \n");
                    exit(-1);
                }

                fgets(proceso, 30, f_procesos);
                fflush(stdout);
                printf("%s", proceso);
                
                send(fd, proceso, 30, 0);
                close(fd);
            }
            printf("\n");
            sleep(2);
        }

        fclose(f_procesos);

        /* buf[numbytes] = '\0'; */

        /* printf("Mensaje del servidor: %s\n", buf); */
        // muestra el mensaje de bienvenida del servidor = )
    }
    else{
        printf("No se ingreso el ip y puerto por parametro\n");
    }
}
