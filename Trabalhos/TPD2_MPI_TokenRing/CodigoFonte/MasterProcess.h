#ifndef MASTERPROCESS__H
#define MASTERPROCESS__H

#include "GlobalDefinitions.h"

class MasterProcess
{
public: //Methods
    MasterProcess(int proc_n);
    ~MasterProcess(){}

    void run();

private: //Members
    void mainMenu();
    void printOption(enmTagCommand command);

private: //Members
    int m_Proc_n;   //Numero total de processos
    char m_Message[MESSAGE_SIZE]; /* Buffer para as mensagens */    
};

#endif //MASTERPROCESS__H