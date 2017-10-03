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
    int m_Proc_n;
    char m_Message[MESSAGE_SIZE]; /* Buffer para as mensagens */    
};

#endif //MASTERPROCESS__H