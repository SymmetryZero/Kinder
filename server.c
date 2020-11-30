#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


int main(int argc, char **argv){
    if(argc > 1){
        //Definicion de variables 
        int fd, fd2, longitud_cliente, puerto;
        puerto = atoi(argv[1]);

        char proceso[30];

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
            /* A continuación la llamada a accept() */

        if ((fd2 = accept(fd, (struct sockaddr *)&client, &longitud_cliente)) == -1){
            printf("error an accept()\n");
            exit(-1);
        }

        while(1){
            
            longitud_cliente = sizeof(struct sockaddr_in);
            /* send(fd2, "Bienvenido a mi servidor. \n", 26, 0); */
            recv(fd2, proceso, 30, 0);
            printf("%s", proceso);
        }
            close(fd2);
        /* close(fd); */
    }else
    {
        printf("NO se ingreso el puerto por parametro\n");
    }
    return 0;

}
