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

    // int tag = 50; /* Tag para as mensagens */
    // MPI_Status status; /* Status de retorno */

    //     std::cout << "O mestre chegou!" << std::endl << "Hoje la na casa do seu Ze vai rolar uma ousadia!" << std::endl;
    //     for (int source = 1; source < m_Proc_n; source++)
    //     {
    //         MPI_Recv(m_Message, MESSAGE_SIZE, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
    //         printf("%s\n", m_Message);
    //     }
}



void MasterProcess::mainMenu()
{
    std::cout << "Menu Principal - Escolha a operacao desejada:" << std::endl;


    for(int command = 0; command < commDef::enmTagCommand__EndOfList; ++command)
    {
        printOption(static_cast<commDef::enmTagCommand>(command));
    }
}


void MasterProcess::printOption(commDef::enmTagCommand command)
{
    switch(command)
    {
        case commDef::enmTagCommand__KillProcess:
            std::cout << command << " - KillProcess - Encerrar execucao do programa." << std::endl;
        break;

        case commDef::enmTagCommand__Master_StartElection:
            std::cout << command << " - StartElection - Iniciar eleicao." << std::endl;
        break;        

        default:
        break;
    }
}