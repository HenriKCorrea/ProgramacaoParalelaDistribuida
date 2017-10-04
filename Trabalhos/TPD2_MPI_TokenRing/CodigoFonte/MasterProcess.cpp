#include "MasterProcess.h"


#include "GlobalDefinitions.h"

#include <mpi.h>

#include <string>
#include <stdio.h>
#include <iostream>
#include <sstream>

MasterProcess::MasterProcess(int proc_n)
{
        m_Proc_n = proc_n;
        
        //Clear message buffer
        memset(m_Message, 0, MESSAGE_SIZE);

        std::cout << "Programa inicializado com sucesso." << std::endl;
}


void MasterProcess::run()
{
    
    enmTagCommand command = enmTagCommand__KillProcess;

    do
    {
        //Call main screen to the user
        command = mainMenu();

        //Parse command
        if(command != enmTagCommand__KillProcess)
        {
            callCommand(command);
        }

    }while(command != enmTagCommand__KillProcess);

    
}


enmTagCommand MasterProcess::mainMenu()
{
    std::cout << "Menu Principal - Escolha a operacao desejada:" << std::endl;

    for(int command = 0; command < enmTagCommand__EndOfList; ++command)
    {
        printOption(static_cast<enmTagCommand>(command));
    }

    int answer;
    enmTagCommand commandAnswer = enmTagCommand__KillProcess;

    std::cin >> answer;
    commandAnswer = static_cast<enmTagCommand>(answer);
    

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

void MasterProcess::callCommand(enmTagCommand command)
{
    switch(command)
    {
        case enmTagCommand__Master_StartElection:
            startElection();
        break;

        default:
        //FAIL
        break;
    }
}

void MasterProcess::startElection()
{
    //Ask the user for the process he wants to exclude from election
    std::cout << "Informe o número dos processos que deseja excluir da eleição (0 para terminar)" << std::endl;
    int processId;
    std::stringstream tmpBuffer;

    do
    {
        std::cin >> processId;
        tmpBuffer << processId;
    }while (processId != 0);

    std::cout << "Dados recebidos: " << tmpBuffer.str() << std::endl;

    //Ask process to start election
    std::cout << "Informe o processo que iniciara a eleição:" << std::endl;
    std::cin >> processId;
    tmpBuffer << processId;

    //Send message
    memcpy(m_Message, tmpBuffer.str().c_str(), tmpBuffer.str().size()); //Set message payload
    std::cout << "Mensagem: " << m_Message << std::endl;
    sendMPIMessage(enmTagCommand__Master_StartElection, processId);
}

void MasterProcess::sendMPIMessage(enmTagCommand commandTag, int destProcessId)
{
    MPI_Send(m_Message, strlen(m_Message) + 1, MPI_CHAR, destProcessId, commandTag, MPI_COMM_WORLD);
}