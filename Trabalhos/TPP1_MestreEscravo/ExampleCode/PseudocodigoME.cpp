#include <stdio.h>
#include <mpi.h>
#include <string.h>

#include "MasterProcess.h"
#include "SlaveProcess.h"







// #define TAREFAS 7; // Numero de tarefas no saco de trabalho para np = 8, processo 0 é o mestre

// int my_rank;       // Identificador deste processo
// int proc_n;        // Numero de processos disparados pelo usuário na linha de comando (np) 
// int message;       // Buffer para as mensagens  
// int saco[TAREFAS]; // saco de trabalho


// MPI_Init(); // funcao que inicializa o MPI, todo o código paralelo esta abaixo

// my_rank = MPI_Comm_rank();  // pega pega o numero do processo atual (rank)
// proc_n  = MPI_Comm_size();  // pega informação do numero de processos (quantidade total)



// if ( my_rank == 0 ) // qual o meu papel: sou o mestre ou um dos escravos?
//    {
//    // papel do mestre

//    for ( i=0 ; i < TAREFAS ; i++) // mando o trabalho para os escravos fazerem
//        {
//        message = saco[i];
//        MPI_Send(&message, i+1); // envio trabalho saco[i] para escravo com id = i+1;
//        }  

//     // recebo o resultado

//     for ( i=0 ; i < TAREFAS ; i++)
//         {
//         // recebo mensagens de qualquer emissor e com qualquer etiqueta (TAG)

//         MPI_Recv(&message, MPI_ANY_SOURCE, MPI_ANY_TAG, status);  // recebo por ordem de chegada com any_source

//         saco[status.MPI_SOURCE-1] = message;   // coloco mensagem no saco na posição do escravo emissor
//         }
//      }               
// else                
//      {
//      // papel do escravo

//      MPI_Recv(&message, 0);    // recebo do mestre

//      message = message+1;      // icremento conteúdo da mensagem

//      MPI_Send(&message, 0);    // retorno resultado para o mestre
//      }

// MPI_Finalize();










#define ARRAY_SIZE 40      // trabalho final com o valores 10.000, 100.000, 1.000.000
#define TAREFAS 7; // Numero de tarefas no saco de trabalho para np = 8, processo 0 é o mestre

// int my_rank;       // Identificador deste processo
// int proc_n;        // Numero de processos disparados pelo usuário na linha de comando (np) 
// int message;       // Buffer para as mensagens  
// int saco[TAREFAS]; // saco de trabalho


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
*   Program start.
*/
int main(int argc, char **argv)
{
int my_rank;       // Identificador deste processo
int proc_n;        // Numero de processos disparados pelo usuário na linha de comando (np) 

int message[ARRAY_SIZE];       // Buffer para as mensagens  
int saco[TAREFAS][ARRAY_SIZE]; // saco de trabalho

/* init array with worst case for sorting */
for (int i=0 ; i < TAREFAS; ++i)
{
    for (int j=0 ; j < TAREFAS; ++j)
    {
        saco[i][j] = ARRAY_SIZE-j;
    }
}




MPI_Init(&argc , & argv);
MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

if (my_rank != 0)
{
    // papel do mestre

    for ( i=0 ; i < TAREFAS ; i++) // mando o trabalho para os escravos fazerem
    {
        memcpy(message)        
        message = saco[i];
        MPI_Send(&message, i+1); // envio trabalho saco[i] para escravo com id = i+1;
        MPI_Send(message, strlen(m_Message) + 1, MPI_CHAR, destProcessId, commandTag, MPI_COMM_WORLD);
    }  

    // recebo o resultado

    for ( i=0 ; i < TAREFAS ; i++)
        {
        // recebo mensagens de qualquer emissor e com qualquer etiqueta (TAG)

        MPI_Recv(&message, MPI_ANY_SOURCE, MPI_ANY_TAG, status);  // recebo por ordem de chegada com any_source

        saco[status.MPI_SOURCE-1] = message;   // coloco mensagem no saco na posição do escravo emissor
//         }
}
else
{
    //Debug process
    MasterProcess process(proc_n);
    process.run();
}
MPI_Barrier(MPI_COMM_WORLD); //Finish program only when all process close execution
MPI_Finalize();
}



