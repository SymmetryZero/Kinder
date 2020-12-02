#include "generarArchivoAleatorio.h"

void generarArchivoDeProcesosPorLotes(){
    FILE *fp, *nombres;

    // Este está entre M y N
    int numeroTotalProcesos = MAX_PROCESOS;
    int tiempoProceso;
    int cantidadProcesosPorLote;
    int contadorProcesos = 0;
    int priority;
    char nombresParaProcesos[200][200];
    int contador = 0;

    // Creando los nombres para cada lote de procesos
    nombres = fopen("nombres.txt","r");
    if (nombres == NULL)
        exit(1);
    else {
        while (feof(nombres) == 0) {
            fgets(nombresParaProcesos[contador],100,nombres);
            nombresParaProcesos[contador][strcspn(nombresParaProcesos[contador], "\r\n")] = 0;
            contador ++;
        }
    }
    fclose(nombres);

    // Creando el archivo para leer los lotes
    fp = fopen ( "procesos.txt", "w" );
    srand(time(NULL));
    contador = 0;
    while (contadorProcesos < numeroTotalProcesos){
        cantidadProcesosPorLote = MAX_LOTES;
        for (int j = 0; j < cantidadProcesosPorLote && contadorProcesos < numeroTotalProcesos; j++){
            tiempoProceso = rand () % (30-1+1) + 1;
            priority = rand () % 6;
            fprintf(fp,"%s%d %d %d\n",nombresParaProcesos[contador], contadorProcesos, tiempoProceso, priority);
            contadorProcesos ++;
        }
        contador ++;
    }


    fclose ( fp );
}
