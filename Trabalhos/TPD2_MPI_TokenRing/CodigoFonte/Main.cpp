#include <stdio.h>
#include <mpi.h>
#include <string.h>

#include "MasterProcess.h"
#include "SlaveProcess.h"

int main(int argc, char **argv)
{
    int my_rank;  /* Identificador do processo */
    int proc_n;   /* Número de processos */
MPI_Init(&argc , & argv);
MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

if (my_rank != 0)
{
    //Token ring process
    SlaveProcess process(false, my_rank, proc_n);
    process.run();
}
else
{
    //Debug process
    MasterProcess process(proc_n);
    process.run();
}
MPI_Finalize();
}
