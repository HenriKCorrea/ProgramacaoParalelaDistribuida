#ifndef SLAVEPROCESS__H
#define SLAVEPROCESS__H

#include "GlobalDefinitions.h"

#include <iostream>
#include <fstream>
#include <vector>

class SlaveProcess
{
public: //Methods
    SlaveProcess(bool isCoordinator, int myRank, int proc_n);
    ~SlaveProcess(){}

    void run();

private: //Methods
    void callCommand(enmTagCommand command);
    int election_GetNextProcess(int myRank, std::vector<int> *excludeList);

    /***********************/
    /* command methods     */
    /***********************/
    void killProcess();
    void startElection();
    void masterStartElection();

    /*****************************/
    /* communication methods     */
    /*****************************/

    void sendMPIMessage(enmTagCommand commandTag, int destProcessId);
    enmTagCommand receiveMPIMessage(int source);


private:    //Members

    int _getNextRank(int myRank);

    bool m_IsCoordinator;
    int m_MyRank;
    int m_Proc_n;
    char m_Message[MESSAGE_SIZE]; /* Buffer para as mensagens */

    int m_NextProcess; //Next process rank to send messages

    int m_Priority;   //Prioridade utilizada durante eleições. Deve ser unico entre todas as classes

    std::ofstream m_OutLogFile;
};

#endif //SLAVEPROCESS__H