#include "MasterProcess.h"


#include "GlobalDefinitions.h"

#include <mpi.h>

#include <string.h>
#include <stdio.h>
#include <iostream>

MasterProcess::MasterProcess(int proc_n)
{
        m_Proc_n = proc_n;
        
        //Clear message buffer
        memset(m_Message, 0, MESSAGE_SIZE);

        std::cout << "Programa inicializado com sucesso." << std::endl;
}


void MasterProcess::run()
{
    
    mainMenu();

}





void MasterProcess::mainMenu()
{
    std::cout << "Menu Principal - Escolha a operacao desejada:" << std::endl;

    for(int command = 0; command < enmTagCommand__EndOfList; ++command)
    {
        printOption(static_cast<enmTagCommand>(command));
    }
}

void MasterProcess::printOption(enmTagCommand command)
{
    switch(command)
    {
        case enmTagCommand__KillProcess:
            std::cout << command << " - KillProcess - Encerrar execucao do programa." << std::endl;
        break;

        case enmTagCommand__Master_StartElection:
            std::cout << command << " - StartElection - Iniciar eleicao." << std::endl;
        break;        

        default:
        break;
    }
}