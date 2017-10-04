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

        std::cout << "Numero do processo: " << m_MyRank << " - Proximo: " << m_NextProcess << std::endl;
        
        memset(m_Message, 0, MESSAGE_SIZE);
}


void SlaveProcess::run()
{
       
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