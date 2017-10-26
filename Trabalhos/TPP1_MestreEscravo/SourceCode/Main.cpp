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

/*
*   Quicksort compare function
*/

int compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
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
    if(argc < 3)
    {
        if(my_rank == 0)
        {
            fprintf(stderr, "Uso:\t%s <tamanho tarefa (vetor)> <numero de tarefas (tamanho saco)> [-qsort]", argv[0]);
            result = 1;
        }
    }
    else 
    {
        //Check if quicksort is set
        bool isQuickSortSet(false);
        if(argc >= 4)
        {
            if(strcmp(argv[3], "-qsort"))
            {   
                isQuickSortSet = true;
            }
        }
        //Get work array size and number of tasks
        size_t ARRAY_SIZE = atoi(argv[1]);
        size_t TAREFAS = atoi(argv[2]);

        //Create message buffer
        int *message = new int[ARRAY_SIZE];       // Buffer para as mensagens  
        int *pMessage;

        //Init master and slave process
        if (my_rank == 0)
        {
            //Master process

            //print quicksort flag
            if(isQuickSortSet)
            {
                printf("Algoritmo de ordenação selecionado: Quick Sort\n");
            }
            else
            {
                printf("Algoritmo de ordenação selecionado: Bubble Sort\n");
            }
    

            //Create work stack
            SacoDeTrabalho saco(ARRAY_SIZE, TAREFAS, proc_n);

            // printf("Saco de trabalho criado. Valor inicial:\n");
            // saco.printSaco();

            //Start clock to measure time
            double t1, t2;
            t1 = MPI_Wtime();        // contagem de tempo inicia neste ponto

            // mando o trabalho para os escravos fazerem
            for(int i = 1; (i < saco.m_Proc_n) && (i < saco.m_NumberOfTasks); ++i)
            {
                pMessage = saco.getNextTaskForSlave(i);
                MPI_Send(pMessage, saco.m_ArraySize, MPI_INT, i, enmTagCommand__SendVector, MPI_COMM_WORLD);
            }

            
            MPI_Status status; /* Return status*/
            while(saco.m_NumOfTasksCompleted < saco.m_NumberOfTasks)
            {
                MPI_Probe(MPI_ANY_SOURCE, enmTagCommand__SendVector, MPI_COMM_WORLD, &status);
                saco.setCompletedTaskInStack(&status);  /*MPI_Recv called inside this function*/
                //MPI_Recv(message, saco.m_ArraySize, MPI_INT, MPI_ANY_SOURCE, enmTagCommand__SendVector, MPI_COMM_WORLD, &status);
                //saco.setCompletedTaskInStack(message, status.MPI_SOURCE);
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

            //Stop clock
            t2 = MPI_Wtime();        // contagem de tempo termina neste ponto


            //Kill all remaining slaves
            for(int i = 1; i < saco.m_Proc_n; ++i)
                MPI_Send(message, 1, MPI_INT, i, enmTagCommand__KillProcess, MPI_COMM_WORLD);



            //saco shall be sorted at this point
            // printf("Ordenação do saco de trabalho concluído! Resultado:\n");
            // saco.printSaco();  
            
            //Print time result
            printf("Tempo de execução do algoritmo: %f\n", t2-t1);
        }
        else
        {
            //Slave Process
            MPI_Status status; /* Return status*/
            do
            {
                //Get work from master
                MPI_Recv(message, ARRAY_SIZE, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                if(status.MPI_TAG == enmTagCommand__SendVector)
                {
                    //printf("Processo %d recebeu uma nova tarefa!\n", my_rank);
                    //Sort vector
                    if(isQuickSortSet)
                    {
                        qsort (message, ARRAY_SIZE, sizeof(int), compare);
                    }
                    else
                    {
                        bs(ARRAY_SIZE, message);
                    }
                    //Send vector for master and ask for more work
                    MPI_Send(message, ARRAY_SIZE, MPI_INT, status.MPI_SOURCE, enmTagCommand__SendVector, MPI_COMM_WORLD);
                }
            }while(status.MPI_TAG != enmTagCommand__KillProcess);

            //printf("Processo %d cometeu suicidio!\n", my_rank);
        }     

        delete(message);
    }

    MPI_Barrier(MPI_COMM_WORLD); //Finish program only when all process close execution
    MPI_Finalize();

    return result;
}
