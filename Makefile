all: despachador server client


client:
	gcc client.c generarArchivoAleatorio.c -o client

server:
	gcc server.c mem_comp.c Semaforos.c -o server

despachador:
	gcc despachador.c Semaforos.c mem_comp.c -o despachador
