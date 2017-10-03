#include <stdio.h>
#include <mpi.h>
#include <string.h>

#include "MasterProcess.h"
#include "SlaveProcess.h"

int main(int argc, char **argv)
{
    int my_rank;  /* Identificador do processo */
    int proc_n;   /* Número de processos */
    int source;   /* Identificador do proc.origem */
    int dest;     /* Identificador do proc.destino */
    int tag = 50; /* Tag para as mensagens */
char message[100]; /* Buffer para as mensagens */
MPI_Status status; /* Status de retorno */
MPI_Init(&argc , & argv);
MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

if (my_rank != 0)
{

    SlaveProcess process = new SlaveProcess();
    process.run();

}
else
{

    MasterProcess process = new MasterProcess();
    process.run();
}
MPI_Finalize();
}
