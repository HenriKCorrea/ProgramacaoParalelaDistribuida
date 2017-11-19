#include <stdio.h>
#include <mpi.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

//#include "SacoDeTrabalho.h"

// #include "MasterProcess.h"
// #include "SlaveProcess.h"

#include <iostream>

#include "DebugMacros.h"
#include <assert.h>

/*
*   MPI Process tags
*/
enum enmTagCommand
{
    enmTagCommand__KillProcess = 0,
    enmTagCommand__SendVector,
};

/*
*   Global flags
*/
class globalFlags{
//private:
public:
    static bool quickSortSet;
    static int arraySize;
    static int delta;
    static int localSortRate;

    static bool parseInputArgs(int argc, char **argv, int myRank)
    {
        bool result(true);

        if(argc < 3)
        {
            //Invalid arg size: return false and print usage message
            result = false;
            if(myRank == 0)
            {
                fprintf(stderr, "Uso:\t%s <tamanho tarefa (vetor)> <delta> [-qsort] [-ls <valor em %% entre 0 ~ 100>]\n", argv[0]); 
            }
        }
        else
        {
            //Get mandatory arg (vector size and delta value) 
            arraySize = atoi(argv[1]);
            delta = atoi(argv[2]);

            //read optional args
            for(int i = 3; i < argc; ++i)
            {
                if(strcmp(argv[i], "-qsort") == 0)
                {
                    quickSortSet = true;
                }
                if((strcmp(argv[i], "-ls") == 0) && ((i + 1) < argc))
                {
                    localSortRate = atoi(argv[i + 1]);
                    if((localSortRate < 0) || (localSortRate > 100))
                    {
                        localSortRate = 0;
                    }
                }
            }
        }
        return result;
    }
};
//Global flags members initialization
bool globalFlags::quickSortSet = false;
int globalFlags::arraySize = 0;
int globalFlags::delta = 0;
int globalFlags::localSortRate = 0;
///////////

/*
*   Node class
*/
class Node
{
public:
    int leftChildRank;
    int rightChildRank;

    Node(int my_rank, int proc_n)
    {
        getChildRank(my_rank, proc_n);
    }

private:
    void getChildRank(int my_Rank, int proc_n)
    {
        int processNumber = my_Rank + 1; 
        int myTreeLevel = getNodeLevel(processNumber);
        int maxTreeLevel = getNodeLevel(proc_n);
        if(myTreeLevel < maxTreeLevel)
        {
            leftChildRank = (my_Rank * 2) + 1;
            rightChildRank = leftChildRank + 1;
        }
        else
        {
            //Im a leaf in ths tree, so i don't have childs
            leftChildRank = rightChildRank = -1;
        }
    }

    int getNodeLevel(int processNum)
    {
        if(processNum <= 1)
        {
            return 0;
        }
        else
        {
            int level = getNodeLevel(processNum >> 1);
            return level + 1;
        }
    }
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

/*
*   Populate root process array with the worst case for sorting  
*/
void initializeVector(int *vector, int arraySize)
{
    for (int i = 0 ; i < arraySize; ++i)              /* init array with worst case for sorting */
        vector[i] = arraySize - i;
    
    DBG_VECTOR(vector, arraySize);
}


/*
* recebe um ponteiro para um vetor que contem as mensagens recebidas dos filhos e            
* intercala estes valores em um terceiro vetor auxiliar. Devolve um ponteiro para este vetor 
*/
int *interleaving(int *vetor, int tam, int tam_vetor_esquera)
{
	int *vetor_auxiliar;
	int i1, i2, i_aux;

	vetor_auxiliar = new int[tam];//(int *)malloc(sizeof(int) * tam);

	i1 = 0;
	i2 = tam_vetor_esquera;

	for (i_aux = 0; i_aux < tam; i_aux++) {
		if (((vetor[i1] <= vetor[i2]) && (i1 < (tam / 2)))
		    || (i2 == tam))
			vetor_auxiliar[i_aux] = vetor[i1++];
		else
			vetor_auxiliar[i_aux] = vetor[i2++];
    }
    
    delete[] vetor;

	return vetor_auxiliar;
}

int *interleaving(int *vetor, int tam, int tam_vetor_local, int tam_vetor_esquera)
{
	int *vetor_auxiliar;
	int i1, i2, i3, i_aux;

	vetor_auxiliar = new int[tam];//(int *)malloc(sizeof(int) * tam);

	i1 = 0;
    i2 = tam_vetor_local;
    i3 = tam_vetor_local + tam_vetor_esquera;
    
    for(i_aux = 0; i_aux < tam; i_aux++)
    {
        if (((vetor[i1] <= vetor[i2]) && (vetor[i1] <= vetor[i3]) && (i1 < tam_vetor_local)) ||
                ((i2 == (tam_vetor_local + tam_vetor_esquera)) && (i3 == tam)))
        {
            vetor_auxiliar[i_aux] = vetor[i1++];
        }
        else
        {
            if (((vetor[i2] <= vetor[i3]) && (i2 < (tam_vetor_local + tam_vetor_esquera)))
                || (i3 == tam))
                vetor_auxiliar[i_aux] = vetor[i2++];
            else
                vetor_auxiliar[i_aux] = vetor[i3++];            
        }
    }
    
    delete[] vetor;

	return vetor_auxiliar;
}

int main(int argc, char **argv)
{
    int my_rank;  /* Identificador do processo */
    int proc_n;   /* Número de processos */
    int result = 0; /* Resultado a ser retornado pelo programa */
    double t1, t2; //temporizadores para medir tempo de execução do algoritmo
    MPI_Status status_pai, status_filhos;

    MPI_Init(&argc , &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

    //Parse input arguments
    bool isArgsValid = globalFlags::parseInputArgs(argc, argv, my_rank);

    if(isArgsValid)
    {
        int tam_vetor = globalFlags::arraySize;     //Vector size may be different for each process
        int *vetor = new int[tam_vetor];           //pointer to data vector
        Node processNode(my_rank, proc_n);          //Discover left and right child process ranks

        if(my_rank != 0)
        {
            //Não sou raiz, tenho pai
            MPI_Recv(vetor, tam_vetor, MPI_INT, MPI_ANY_SOURCE, enmTagCommand__SendVector, MPI_COMM_WORLD, &status_pai); 
            MPI_Get_count(&status_pai, MPI_INT, &tam_vetor);  // descubro tamanho da mensagem recebida
        }
        else
        {
            //Root process
            initializeVector(vetor, tam_vetor);
            printf("Algoritmo de ordenação selecionado: ");
            if(globalFlags::quickSortSet)
            {
                printf("QuickSort\n");
            }
            else
            {
                printf("BubbleSort\n");
            }
            printf("Percentual ordenação local: %d%%\n", globalFlags::localSortRate);
            t1 = MPI_Wtime();        // contagem de tempo inicia neste ponto
        }

        // dividir ou conquistar?
        if ( tam_vetor <= globalFlags::delta )
        {
            // conquisto
            //verifica se deve executar algorítmo bubble ou quick sort
            if(globalFlags::quickSortSet)
            {
                qsort(vetor, tam_vetor, sizeof(int), compare);
            }
            else
            {
                bs(tam_vetor, vetor);  
            }
        }
        else
        {
            // Dividir
            // quebrar em duas partes e mandar para os filhos

            if(globalFlags::localSortRate == 0)
            {
                //Não realiza ordenação local. Manda todo o trabalho para os filhos.
                int *vetor_esquerda = &vetor[0];
                int tam_vetor_esquerda = tam_vetor/2;
                int *vetor_direita = &vetor[tam_vetor_esquerda];
                int tam_vetor_direita = tam_vetor - tam_vetor_esquerda;

                MPI_Send (vetor_esquerda, tam_vetor_esquerda, MPI_INT, processNode.leftChildRank, enmTagCommand__SendVector, MPI_COMM_WORLD);  // mando metade inicial do vetor
                MPI_Send (vetor_direita, tam_vetor_direita, MPI_INT,processNode.rightChildRank, enmTagCommand__SendVector, MPI_COMM_WORLD);  // mando metade final
            
                // receber dos filhos
                MPI_Recv(vetor_esquerda, tam_vetor_esquerda, MPI_INT, processNode.leftChildRank, enmTagCommand__SendVector, MPI_COMM_WORLD, &status_filhos);
                MPI_Recv(vetor_direita, tam_vetor_direita, MPI_INT, processNode.rightChildRank, enmTagCommand__SendVector, MPI_COMM_WORLD, &status_filhos);        

                //Intercala os vetores recebidos
                vetor = interleaving(vetor, tam_vetor, tam_vetor_esquerda);
            }
            else
            {
                //Ordenação local ativado. Pai separa parte do trabalho para ordenar e envia o restante para os filhos (mesma carga para ambos)

                /*
                *   **********************************************************
                *   *Vetor_local*    Vetor_Esquerda    *    Vetor_Direita    *
                *   **********************************************************
                */

                int *vetor_local = &vetor[0];
                int tam_vetor_local = ((globalFlags::localSortRate / 100.0) * tam_vetor); //Regra de três
                int *vetor_esquerda = &vetor[tam_vetor_local];
                int tam_vetor_esquerda = (tam_vetor - tam_vetor_local)/2;
                int *vetor_direita = &vetor[tam_vetor_local + tam_vetor_esquerda];
                int tam_vetor_direita = tam_vetor - tam_vetor_local - tam_vetor_esquerda;                

                MPI_Send (vetor_esquerda, tam_vetor_esquerda, MPI_INT, processNode.leftChildRank, enmTagCommand__SendVector, MPI_COMM_WORLD);  // mando metade inicial do vetor
                MPI_Send (vetor_direita, tam_vetor_direita, MPI_INT,processNode.rightChildRank, enmTagCommand__SendVector, MPI_COMM_WORLD);  // mando metade final

                //verifica se deve executar algorítmo bubble ou quick sort
                if(globalFlags::quickSortSet)
                {
                    qsort(vetor_local, tam_vetor_local, sizeof(int), compare);
                }
                else
                {
                    bs(tam_vetor_local, vetor_local);
                }
            
                // receber dos filhos
                MPI_Recv(vetor_esquerda, tam_vetor_esquerda, MPI_INT, processNode.leftChildRank, enmTagCommand__SendVector, MPI_COMM_WORLD, &status_filhos);
                MPI_Recv(vetor_direita, tam_vetor_direita, MPI_INT, processNode.rightChildRank, enmTagCommand__SendVector, MPI_COMM_WORLD, &status_filhos);        

                //Intercala os vetores recebidos
                //vetor = interleaving(vetor, tam_vetor, tam_vetor_local, tam_vetor_esquerda);   
                qsort(vetor, tam_vetor, sizeof(int), compare);
            }

        }

        if ( my_rank != 0 )
        {
            // mando para o pai
            MPI_Send(vetor, tam_vetor, MPI_INT, status_pai.MPI_SOURCE, enmTagCommand__SendVector, MPI_COMM_WORLD);
        }
        else
        {
            t2 = MPI_Wtime();                                       // contagem de tempo termina neste ponto
            DBG_VECTOR(vetor, tam_vetor);                           // sou o raiz, mostro vetor
            printf("Tempo de execução do algoritmo: %f segundos\n", t2-t1);  // imprime tempo de execução

        }

        MPI_Barrier(MPI_COMM_WORLD); //Finish program only when all process close execution
        MPI_Finalize();                    
    }

    return 0;
}
