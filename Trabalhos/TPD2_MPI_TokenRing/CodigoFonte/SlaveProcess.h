#ifndef SLAVEPROCESS__H
#define SLAVEPROCESS__H

#include "GlobalDefinitions.h"

class SlaveProcess
{
public: //Methods
    SlaveProcess(bool isCoordinator, int myRank, int proc_n);
    ~SlaveProcess(){}

    void run();

private:    //Members
    bool m_IsCoordinator;
    int m_MyRank;
    int m_Proc_n;
    char m_Message[MESSAGE_SIZE]; /* Buffer para as mensagens */

    int m_Priority;   //Prioridade utilizada durante eleições. Deve ser unico entre todas as classes
};

#endif //SLAVEPROCESS__H