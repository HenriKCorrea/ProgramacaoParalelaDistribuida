#include <stdio.h>
#include <mpi.h>
#include <string.h>
#include <stdlib.h>


#include <iostream>

#include "DebugMacros.h"

/*
*   MPI Process tags
*/
enum enmTagCommand
{
    enmTagCommand__KillProcess = 0,
    enmTagCommand__SendVector,
    enmTagCommand__SendBuffer
};

/*
*   Global flags
*/
class globalFlags{
//private:
public:
    static int arraySize;
    static double bufferSizePercent;
    static bool isQuickSortSet;

    static bool parseInputArgs(int argc, char **argv, int myRank)
    {
        bool result(true);

        if(argc < 3)
        {
            //Invalid arg size: return false and print usage message
            result = false;
            if(myRank == 0)
            {
                fprintf(stderr, "Uso:\t%s <tamanho tarefa (vetor)> <buffer em %% enviado entre process [0 ~ 100]> [-qsort]\n", argv[0]); 
            }
        }
        else
        {
            //Get mandatory arg (vector size and delta value) 
            arraySize = atoi(argv[1]);
            int tmpBufferSize = atoi(argv[2]);
            if((tmpBufferSize >= 0) || (tmpBufferSize <= 100))
            {
                bufferSizePercent = tmpBufferSize / 100.0;
                    for(int i = 3; i < argc; ++i)
                    {
                        if(strcmp(argv[i], "-qsort") == 0)
                            isQuickSortSet = true;
                    }

                if(myRank == 0)
                {
                    printf("Percentual buffer: %.0f%%\n", bufferSizePercent * 100);
                    printf("Algoritmo de ordenação selecionado: ");
                    if(isQuickSortSet)
                        printf("Quick Sort\n");
                    else
                        printf("Bubble Sort\n");       
                }
            }   
            else
            {
                result = false;
                if(myRank == 0)
                {
                    fprintf(stderr, "Uso:\t%s <tamanho tarefa (vetor)> <buffer em %% enviado entre process [0 ~ 100] [-qsort]>\n", argv[0]); 
                }
            }
        }
        return result;
    }
};
//Global flags members initialization
int globalFlags::arraySize = 0;
double globalFlags::bufferSizePercent = 0.0;
bool globalFlags::isQuickSortSet = false;
///////////

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

/*
*   Populate root process array with the worst case for sorting  
*/
int initializeVector(int **vector, int arraySize, int my_rank, int proc_n, double bufferSizePercent)
{
    int tam_vetor_local = arraySize / proc_n;
    int offset_valor = (proc_n - my_rank) * tam_vetor_local;
    int bufferSize = tam_vetor_local* bufferSizePercent;
    
    *vector = new int[tam_vetor_local + bufferSize];
    for (int i = 0 ; i < tam_vetor_local; ++i)              /* init array with worst case for sorting */
        (*vector)[i] = offset_valor - i;
    
    return tam_vetor_local;
}

int main(int argc, char **argv)
{
    int my_rank;  /* Identificador do processo */
    int proc_n;   /* Número de processos */
    int result = 0; /* Resultado a ser retornado pelo programa */
    double t1, t2; //temporizadores para medir tempo de execução do algoritmo


    MPI_Init(&argc , &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

    //Parse input arguments
    bool isArgsValid = globalFlags::parseInputArgs(argc, argv, my_rank);

    if(isArgsValid)
    {
        int *vetor;                                                                              //pointer to data vector
        int tam_vetor = initializeVector(&vetor, globalFlags::arraySize, my_rank, proc_n, globalFlags::bufferSizePercent); //Vector size may be different for each process
        int tam_buffer = tam_vetor * globalFlags::bufferSizePercent;

        #ifdef DEBUG
        for(int i = 0; i < proc_n; ++i)
        {
            MPI_Barrier(MPI_COMM_WORLD);
            if(my_rank == i)
            {
                DBG_VAR(my_rank);
                DBG_VECTOR(vetor, tam_vetor);
            }
        }
        #endif        
        
        int i_max_valor = tam_vetor - 1;
        int proc_direita = my_rank + 1;
        int proc_esquerda = my_rank - 1;
        int element_state_verify;
        int *estado = new int[proc_n];      //array usado para comparar o estado atual do processo com seu vizinho
        bool pronto(false);

        MPI_Barrier(MPI_COMM_WORLD);        //Sincroniza todos os processos para iniciar contagem do algoritmo
        if(my_rank == 0)             // processo 0 é o encarregado de controlar o cronômetro
        {
            t1 = MPI_Wtime();        // contagem de tempo inicia neste ponto
        }

        ////////////////////
        // Inicia algorítmo
        ///////////////////
        while(!pronto)
        {

        // ordeno vetor local
            if(globalFlags::isQuickSortSet)
                qsort(vetor, tam_vetor, sizeof(int), compare);
            else
                bs(tam_vetor, vetor); 

        // verifico condição de parada

            // se não for np-1, mando o meu maior elemento para a direita
            if(my_rank != (proc_n - 1))
                MPI_Send(&vetor[i_max_valor], 1, MPI_INT, proc_direita, enmTagCommand__SendVector, MPI_COMM_WORLD);
        
            // se não for 0, recebo o maior elemento da esquerda
            if(my_rank != 0)
                MPI_Recv(&element_state_verify, 1, MPI_INT, proc_esquerda, enmTagCommand__SendVector, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            else
            {
                //Sou o processo Zero. Meu estado sempre será verdadeiro.
                element_state_verify = vetor[0] - 1;
            }
                
            // comparo se o meu menor elemento é maior do que o maior elemento recebido (se sim, estou ordenado em relação ao meu vizinho)
            if(vetor[0] > element_state_verify)
                estado[my_rank] = 1;
            else
                estado[my_rank] = 0;
            
            // compartilho o meu estado com todos os processos
            for(int i = 0; i < proc_n; ++i)
                MPI_Bcast(&estado[i], 1, MPI_INT, i, MPI_COMM_WORLD);

            // se todos estiverem ordenados com seus vizinhos, a ordenação do vetor global está pronta ( pronto = TRUE, break)
            pronto = true;
            for(int i = 0; i < proc_n; ++i)
                pronto &= estado[i];            //Se existir pelo menos um estado com valor zero, pronto = FALSE
            
            if(pronto)
                break;

            // senão continuo

        // troco valores para convergir

            // se não for o 0, mando os menores valores do meu vetor para a esquerda
            if(my_rank != 0)
                MPI_Send(&vetor[0], tam_buffer, MPI_INT, proc_esquerda, enmTagCommand__SendBuffer, MPI_COMM_WORLD);



            // se não for np-1, recebo os menores valores da direita
            if(my_rank != (proc_n - 1))
            {
                MPI_Recv(&vetor[tam_vetor], tam_buffer, MPI_INT, proc_direita, enmTagCommand__SendBuffer, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                // ordeno estes valores com a parte mais alta do meu vetor local
                static int offset = tam_vetor - tam_buffer;
                static int buffer_size_2 = tam_buffer * 2; 
                if(globalFlags::isQuickSortSet)                                         //ordena buffer de forma crescente
                    qsort(&vetor[offset], buffer_size_2, sizeof(int), compare);
                else
                    bs(buffer_size_2, &vetor[offset]);                 

                // devolvo os valores que recebi para a direita
                MPI_Send(&vetor[tam_vetor], tam_buffer, MPI_INT, proc_direita, enmTagCommand__SendBuffer, MPI_COMM_WORLD);
            }

            // se não for o 0, recebo de volta os maiores valores da esquerda
            if(my_rank != 0)
                MPI_Recv(&vetor[0], tam_buffer, MPI_INT, proc_esquerda, enmTagCommand__SendBuffer, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        if(my_rank == 0)
        {
            t2 = MPI_Wtime();                                       // contagem de tempo termina neste ponto
            printf("Tempo de execução do algoritmo: %f segundos\n", t2-t1);  // imprime tempo de execução
        }

        #ifdef DEBUG
        for(int i = 0; i < proc_n; ++i)
        {
            MPI_Barrier(MPI_COMM_WORLD);
            if(my_rank == i)
            {
                DBG_VAR(my_rank);
                DBG_VECTOR(vetor, tam_vetor);
            }
        }
        #endif

        delete[](vetor);
        delete[](estado);
    }

    MPI_Barrier(MPI_COMM_WORLD); //Finish program only when all process close execution
    MPI_Finalize();

    return 0;
}
