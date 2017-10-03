#ifndef MASTERPROCESS__H
#define MASTERPROCESS__H

class MasterProcess
{
public: //Methods
    MasterProcess(){}
    ~MasterProcess(){}

    void run();

private: //Members
    

    int my_rank;  /* Identificador do processo */
    int proc_n;   /* Número de processos */
    int source;   /* Identificador do proc.origem */
    int dest;     /* Identificador do proc.destino */
    int tag = 50; /* Tag para as mensagens */
char message[100]; /* Buffer para as mensagens */
MPI_Status status; /* Status de retorno */

};

#endif //MASTERPROCESS__H