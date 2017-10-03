#include "SlaveProcess.h"

void SlaveProcess::run()
{

    
        sprintf(message, "Greetings from process %d!", my_rank);
        printf("Flood do processo %d! Programa tem %d processos", my_rank, proc_n);
        dest = 0;
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
}
