#include <stdio.h>
#include <mpi.h>
#include <string.h>
#include <stdlib.h>

#include "SacoDeTrabalho.h"

// #include "MasterProcess.h"
// #include "SlaveProcess.h"

/*
*   MPI Process tags
*/
enum enmTagCommand
{
    enmTagCommand__KillProcess = 0,
    enmTagCommand__SendVector,
};

/*
*   Bubble Sort for an Integer array of size 'n'.
*/
void bs(int n, int * vetor)
{
    int c=0, d, troca, trocou =1;

    while (c < (n-1) & trocou )
        {
        trocou = 0;
        for (d = 0 ; d < n - c - 1; d++)
            if (vetor[d] > vetor[d+1])
                {
                troca      = vetor[d];
                vetor[d]   = vetor[d+1];
                vetor[d+1] = troca;
                trocou = 1;
                }
        c++;
        }
}

int main(int argc, char **argv)
{
    int my_rank;  /* Identificador do processo */
    int proc_n;   /* Número de processos */
    int result = 0; /* Resultado a ser retornado pelo programa */


    

    MPI_Init(&argc , & argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

    
    //Parse arguments
    if(argc != 3)
    {
        if(my_rank == 0)
        {
            fprintf(stderr, "Uso:\t%s <tamanho tarefa (vetor)> <numero de tarefas (tamanho saco)>", argv[0]);
            result = 1;
        }
    }
    else
    {

        //Get work array size and number of tasks
        size_t ARRAY_SIZE = atoi(argv[1]);
        size_t TAREFAS = atoi(argv[2]);


        //Init master and slave process
        if (my_rank == 0)
        {
            //Master process         

            //Create message buffer
            int *message = new int[ARRAY_SIZE];       // Buffer para as mensagens  
            int *pMessage;

            //Create work stack
            SacoDeTrabalho saco(ARRAY_SIZE, TAREFAS, proc_n);


            // mando o trabalho para os escravos fazerem
            for(int i = 0; (i < saco.m_Proc_n) && (i < saco.m_NumberOfTasks); ++i)
            {
                pMessage = saco.getNextTaskForSlave(i + 1);
                MPI_Send(pMessage, saco.m_ArraySize, MPI_INT, i + 1, enmTagCommand__SendVector, MPI_COMM_WORLD);
            }

            
            MPI_Status status; /* Return status*/
            while(saco.m_NumOfTasksCompleted < saco.m_NumberOfTasks)
            {
                MPI_Recv(message, saco.m_ArraySize, MPI_INT, MPI_ANY_SOURCE, enmTagCommand__SendVector, MPI_COMM_WORLD, &status);
                saco.setCompletedTaskInStack(message, status.MPI_SOURCE);
                pMessage = saco.getNextTaskForSlave(status.MPI_SOURCE);
                if(pMessage == NULL)
                {
                    MPI_Send(message, 1, MPI_INT, status.MPI_SOURCE, enmTagCommand__KillProcess, MPI_COMM_WORLD);
                }
                else
                {
                    MPI_Send(pMessage, saco.m_ArraySize, MPI_INT, status.MPI_SOURCE, enmTagCommand__SendVector, MPI_COMM_WORLD);
                }
            }

            //saco shall be sorted at this point

        }
        else
        {
             //Slave Process

            MPI_Status status; /* Return status*/
            //check if task is sort or kilk

            //do
                //Read vector
                //Sort vector
                //send vector
            //while(tag != kill)
        }        


    }



    MPI_Barrier(MPI_COMM_WORLD); //Finish program only when all process close execution
    MPI_Finalize();

    return result;
}
