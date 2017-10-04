#include "SlaveProcess.h"

#include <mpi.h>

#include <string.h>
#include <stdio.h>
#include <iostream>

SlaveProcess::SlaveProcess(bool isCoordinator, int myRank, int proc_n)
{
        m_IsCoordinator = isCoordinator;
        m_MyRank = myRank;
        m_Priority = myRank;
        m_Proc_n = proc_n;

        //save the default next process rank
        m_NextProcess = _getNextRank(m_MyRank);
        
        memset(m_Message, 0, MESSAGE_SIZE);
}


void SlaveProcess::run()
{
        MPI_Status status; /* Return status*/
        MPI_Recv(m_Message, MESSAGE_SIZE, MPI_CHAR, 0 /*Source process*/, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        std::cout << "Processo " << m_MyRank << " Tag recebida: " << status.MPI_TAG << std::endl;

        //TODO Criar switch case da tag;
        //Verificar se depois do zero a prioridade é igual a sua (quem iniciou eleição).
        //caso não, envia mensagem de eleição para próximo (não pode ser ninguem da lista de excluidos)
        //caso sim, envia uma mensagem contendo o id de quem ganhou a eleição.
        
        //int startElection_GetNextProcess()

        //ElectionWinner
        //Primeira info é a lista de ignorados separado pelo zero.
        //O primeiro ID é de quem transmitiu, o segundo é de quem ganhou.
        //Se ID de vencedor é o seu, seta flag (bool) para true.
        //Quando mensagem chegar novamente no transmissor
}

int SlaveProcess::_getNextRank(int myRank)
{
        int result = myRank + 1;

        //If rank is greather than the number of process, return firtst process rank (circular buffer)
        if(result >= m_Proc_n) 
        {
                result = 1;
        }

        return result;
}