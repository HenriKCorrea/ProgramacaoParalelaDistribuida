#include "SlaveProcess.h"

#include <mpi.h>

#include <string.h>
#include <stdio.h>
#include <iostream>

SlaveProcess::SlaveProcess(bool isCoordinator, int myRank, int proc_n)
{
        m_IsCoordinator = isCoordinator;
        m_MyRank = myRank;
        m_Proc_n = proc_n;
        
        memset(m_Message, 0, MESSAGE_SIZE);
}


void SlaveProcess::run()
{
        sprintf(m_Message, "Greetings from process %d!", m_MyRank);
        printf("Flood do processo %d! Programa tem %d processos", m_MyRank, m_Proc_n);

        int dest = 0; //Processo mestre
        int tag = 50; /* Tag para as mensagens */
        MPI_Send(m_Message, strlen(m_Message) + 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
}
