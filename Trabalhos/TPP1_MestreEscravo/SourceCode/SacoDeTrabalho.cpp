#include "SacoDeTrabalho.h"

#include <string.h>
#include <stdio.h>

SacoDeTrabalho::SacoDeTrabalho(int arraySize, int numberOFTasks, int numberOfProcess)
{
    //Set Members
    m_ArraySize = arraySize;
    m_NumberOfTasks = numberOFTasks;
    m_Proc_n = numberOfProcess;

    m_NumOfTasksRemaining = m_NumberOfTasks;
    m_NumOfTasksInProgress = 0;
    m_NumOfTasksCompleted = 0;

    //Create work stack
    m_Saco = new int*[m_NumberOfTasks];           //Create rows of the matrix
    for(int i = 0; i < m_NumberOfTasks; ++i)
    {
        m_Saco[i] = new int[m_ArraySize];      //Create collumns of the matrix

        /* init array with worst case for sorting */
        for (int j=0 ; j < m_ArraySize; ++j)
        {
            m_Saco[i][j] = m_ArraySize-j;
        }
    }

    //Init processTaskMap
    m_ProcessTaskMap = new int[m_Proc_n];
    memset(m_ProcessTaskMap, -1, sizeof(int) * m_Proc_n); //Initial value is -1
}

SacoDeTrabalho::~SacoDeTrabalho()
{
    for(int i = 0; i < m_NumberOfTasks; ++i)
    {
        delete(m_Saco[i]);  //Delete columns
    }
    delete(m_Saco);;    //Delete rows
    delete(m_ProcessTaskMap);
}

int *SacoDeTrabalho::getNextTaskForSlave(int slaveRank)
{
    int *nextTask = NULL;

    //Get next task only if there's task remaining and the slave is not sorting any array.
    if(m_NumOfTasksRemaining > 0)
    {
        int nextTaskIndex = m_NumOfTasksCompleted + m_NumOfTasksInProgress; //Get next index
        nextTask = m_Saco[nextTaskIndex];                                   //set task ptr

        m_ProcessTaskMap[slaveRank] = nextTaskIndex;    //Associate task index to process
        ++m_NumOfTasksInProgress;   //Increment number of tasks being processed
        --m_NumOfTasksRemaining;    //Decrement number of tasks remaining
    }

    return nextTask;
}

void SacoDeTrabalho::setCompletedTaskInStack(int *task, int slaveRank)
{
    int taskIndex = m_ProcessTaskMap[slaveRank];
    m_ProcessTaskMap[slaveRank] = -1;

    memcpy(m_Saco[taskIndex], task, sizeof(int)*m_ArraySize);
    ++m_NumOfTasksCompleted;
    --m_NumOfTasksInProgress;
}