#ifndef SACO_DE_TRABALHO__H
#define SACO_DE_TRABALHO__H

#include <utility>

class SacoDeTrabalho
{
public:
    /******************/
    /* Public Methods */
    /******************/    
    SacoDeTrabalho(int arraySize, int numberOFTasks, int m_Proc_n);
    ~SacoDeTrabalho();

    int* getNextTaskForSlave(int slaveRank);
    int getTaskIndexInStack(int slaveRank);
    void setCompletedTaskInStack(int *task, int slaveRank);

    void printSaco();

    /******************/
    /* Public Members */
    /******************/     
    int m_NumOfTasksCompleted;
    int m_NumOfTasksInProgress;
    int m_NumOfTasksRemaining;

    int m_ArraySize;
    int m_NumberOfTasks;
    int m_Proc_n;

private:
    /*******************/
    /* Private Members */
    /*******************/


    int **m_Saco;

    int *m_ProcessTaskMap;

    
    /*******************/
    /* Private Methods */
    /*******************/
    //void initProcessTaskMap()


};

#endif //SACO_DE_TRABALHO__H