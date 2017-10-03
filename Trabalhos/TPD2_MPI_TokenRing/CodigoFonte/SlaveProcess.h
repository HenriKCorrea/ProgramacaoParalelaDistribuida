#ifndef SLAVEPROCESS__H
#define SLAVEPROCESS__H

class SlaveProcess
{
public:
    SlaveProcess(){}
    ~SlaveProcess(){}

    void run();

private:
    bool m_IsCoordinator;
    int m_MyRank;

    int my_rank;  /* Identificador do processo */
    int proc_n;   /* Número de processos */
    int source;   /* Identificador do proc.origem */
    int dest;     /* Identificador do proc.destino */
    int tag = 50; /* Tag para as mensagens */
char message[100]; /* Buffer para as mensagens */
MPI_Status status; /* Status de retorno */    
    
};

#endif //SLAVEPROCESS__H