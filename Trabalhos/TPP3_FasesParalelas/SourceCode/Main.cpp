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

    static bool parseInputArgs(int argc, char **argv, int myRank)
    {
        bool result(true);

        if(argc < 3)
        {
            //Invalid arg size: return false and print usage message
            result = false;
            if(myRank == 0)
            {
                fprintf(stderr, "Uso:\t%s <tamanho tarefa (vetor)> <buffer em %% enviado entre process em uma faixa de 0 ~ 100>\n", argv[0]); 
            }
        }
        else
        {
            //Get mandatory arg (vector size and delta value) 
            arraySize = atoi(argv[1]);
            int tmpBufferSize = atoi(argv[2]);
            bufferSizePercent = tmpBufferSize / 100.0;

            if(myRank == 0)
            {
                printf("Percentual buffer: %f\n", bufferSizePercent); 
            }
            
            //read optional args
            // for(int i = 3; i < argc; ++i)
            // {
            //     if(strcmp(argv[i], "-qsort") == 0)
            //     {
            //         quickSortSet = true;
            //     }
            // }
        }
        return result;
    }
};
//Global flags members initialization
int globalFlags::arraySize = 0;
double globalFlags::bufferSizePercent = 0.0;
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
int initializeVector(int **vector, int arraySize, int my_rank, int proc_n)
{
    int tam_vetor_local = arraySize / proc_n;
    int offset_valor = (proc_n - my_rank) * tam_vetor_local;
    
    *vector = new int[tam_vetor_local];
    for (int i = 0 ; i < tam_vetor_local; ++i)              /* init array with worst case for sorting */
        (*vector)[i] = offset_valor - i;
    
    //DBG_VAR(my_rank);
    //DBG_VECTOR((*vector), tam_vetor_local);
    
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
        int tam_vetor = initializeVector(&vetor, globalFlags::arraySize, my_rank, proc_n); //Vector size may be different for each process
        
        int i_max_valor = tam_vetor - 1;
        int proc_direita = my_rank + 1;
        int proc_esquerda = my_rank - 1;
        int tam_buffer = tam_vetor * globalFlags::bufferSizePercent;
        int *buffer = new int[tam_buffer*2];
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
            static int contador(0);

        // ordeno vetor local
            bs(tam_vetor, vetor); 

        // verifico condição de parada

            // se não for np-1, mando o meu maior elemento para a direita
            if(my_rank != (proc_n - 1))
                MPI_Send(&vetor[i_max_valor], 1, MPI_INT, proc_direita, enmTagCommand__SendVector, MPI_COMM_WORLD);
        
            // se não for 0, recebo o maior elemento da esquerda
            if(my_rank != 0)
                MPI_Recv(&buffer[0], 1, MPI_INT, proc_esquerda, enmTagCommand__SendVector, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // comparo se o meu menor elemento é maior do que o maior elemento recebido (se sim, estou ordenado em relação ao meu vizinho)
            if(my_rank != 0)
            {
                if(vetor[0] > buffer[0])
                {
                    estado[my_rank] = 1;
                }
                else
                {
                    estado[my_rank] = 0;
                }
            }
            else
            {
                //Sou o processo número zero
                estado[my_rank] = 1;
            }
            
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
                MPI_Recv(&buffer[tam_buffer], tam_buffer, MPI_INT, proc_direita, enmTagCommand__SendBuffer, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                // ordeno estes valores com a parte mais alta do meu vetor local
                static int offset = tam_vetor - tam_buffer; 
                memcpy(&buffer[0], &vetor[offset], tam_buffer * sizeof(int));          //copia os maiores valres para a parte baixa do buffer
                qsort(buffer, tam_buffer*2, sizeof(int), compare);       //ordena buffer de forma crescente
                memcpy(&vetor[offset], &buffer[0], tam_buffer * sizeof(int));          //copia os menores valores do buffer para os maiores valores do vetor

                // devolvo os valores que recebi para a direita
                MPI_Send(&buffer[tam_buffer], tam_buffer, MPI_INT, proc_direita, enmTagCommand__SendBuffer, MPI_COMM_WORLD);
            }

            // se não for o 0, recebo de volta os maiores valores da esquerda
            if(my_rank != 0)
                MPI_Recv(&vetor[0], tam_buffer, MPI_INT, proc_esquerda, enmTagCommand__SendBuffer, MPI_COMM_WORLD, MPI_STATUS_IGNORE);



        if(contador == 10)
        {
            if(my_rank == 0)
            {
                DBG_VAR(my_rank);
                DBG_VECTOR(estado, proc_n);
                //DBG_VECTOR(vetor, tam_vetor);         
            }

            while(1)
            {
                ;
            }

        }
        else
            ++contador;



        }


        if(my_rank == 0)
        {
            t2 = MPI_Wtime();                                       // contagem de tempo termina neste ponto
            printf("Tempo de execução do algoritmo: %f segundos\n", t2-t1);  // imprime tempo de execução
        }
            
        printf("FEITOOO!\n");
        DBG_VAR(my_rank);
        DBG_VECTOR(vetor, tam_vetor);
        delete[](vetor);
        delete[](buffer);


    }

    MPI_Barrier(MPI_COMM_WORLD); //Finish program only when all process close execution
    MPI_Finalize();

    return 0;
}


            //t1 = MPI_Wtime();        // contagem de tempo inicia neste ponto

            // t2 = MPI_Wtime();                                       // contagem de tempo termina neste ponto
            // DBG_VECTOR(vetor, tam_vetor);                           // sou o raiz, mostro vetor
            // printf("Tempo de execução do algoritmo: %f segundos\n", t2-t1);  // imprime tempo de execução