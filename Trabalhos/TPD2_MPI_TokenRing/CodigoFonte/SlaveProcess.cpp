#include "SlaveProcess.h"

#include <mpi.h>

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <cstdlib>

#include <vector>

SlaveProcess::SlaveProcess(bool isCoordinator, int myRank, int proc_n)
{
        m_IsCoordinator = isCoordinator;
        m_MyRank = myRank;
        m_Priority = myRank;
        m_Proc_n = proc_n;

        //save the default next process rank
        m_NextProcess = _getNextRank(m_MyRank);
        //If this process is the highest rank, set as coordinator
        if(m_NextProcess == 1)
        {
                m_IsCoordinator = true;
        }
        
        memset(m_Message, 0, MESSAGE_SIZE);

        
}


void SlaveProcess::run()
{

        enmTagCommand command = enmTagCommand__KillProcess;
        
        //Create output logfile
        std::stringstream fileName;
        fileName << "./LogOutput/Process" << m_MyRank << "LogOutput.txt";
        m_OutLogFile.open(fileName.str(), std::ofstream::out | std::ofstream::trunc);

        //m_OutLogFile << "Teste da farinha!";

        do
        {
                //Call main screen to the user
                command = receiveMPIMessage(MPI_ANY_SOURCE);

                //Parse command
                callCommand(command);
                

        }while(command != enmTagCommand__KillProcess);

        m_OutLogFile << "Encerrando processo..." << std::endl;
        m_OutLogFile.close();


        //TODO Criar switch case da tag;
        //Verificar se depois do zero a prioridade é igual a sua (quem iniciou eleição).
        //caso não, envia mensagem de eleição para próximo (não pode ser ninguem da lista de excluidos)
        //caso sim, envia uma mensagem contendo o id de quem ganhou a eleição.
        
        //int startElection_GetNextProcess()

        //ElectionWinner
        //Primeira info é a lista de ignorados separado pelo zero.
        //O primeiro ID é de quem transmitiu, o segundo é de quem ganhou.
        //Se ID de vencedor é o seu, seta flag (bool) para true.
        //Quando mensagem chegar novamente no transmissor
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

// int splitMessageString()
// {
//         char *pch
//         int numOfArgs = 0;

//         //Split message eliminating blank spaces
//         pch = strtok (m_Message," ");
//         pch = 0;
//         ++numOfArgs;

//         while (pch != NULL)
//         {
//           printf ("%s\n",pch);
//           pch = strtok (NULL, " ,.-");
//         }        
// }

void SlaveProcess::killProcess()
{
        //Verify if ID is equal
        int lastID = atoi(m_Message);

        //If id is not its own, send kill message to next process
        if(m_MyRank != lastID)
        {
                sendMPIMessage(enmTagCommand__KillProcess, _getNextRank(m_MyRank));
        }
}

void SlaveProcess::masterStartElection()
{
        m_OutLogFile << "MasterStartElection - Dados recebidos" << std::endl;

        char messageCpy[MESSAGE_SIZE] = {};
        memcpy(messageCpy, m_Message, strlen(m_Message));
        
        int processId = -1;

        std::vector<int> ignoreProcessList;
        char *pch = strtok(messageCpy, " ");

        //Extract ignore process list
        m_OutLogFile << "Dados de ignore" << std::endl;
        while ((*pch != '0') && (pch != NULL))
        {
                //m_OutLogFile << "Passo" << std::endl;
                processId = atoi(pch);
                if(processId != 0)
                {
                        ignoreProcessList.push_back(processId);
                        pch = strtok(NULL, " ");
                        m_OutLogFile << processId << std::endl;
                }
        }


        //Extract priority list
        std::vector<int> priorityList;
        int priority = 0;
        pch = strtok(NULL, " ");        // Jump Zero

        while (pch != NULL)
        {
                //m_OutLogFile << "Passo" << std::endl;
                priority = atoi(pch);
                priorityList.push_back(priority);        
                pch = strtok(NULL, " ");
        }

        m_OutLogFile << "Dados da lista Priority" << std::endl;
        std::vector<int>::iterator it = priorityList.begin();
        for(it; it < priorityList.end(); ++it)
        {
                m_OutLogFile << *it << std::endl;
        }


        //Check if all process have answered the election
        bool isMyPriorityInTheList = false;
        for(it = priorityList.begin(); it < priorityList.end(); ++it)
        {
                if(m_Priority == *it)
                {
                        isMyPriorityInTheList = true;
                        break;
                }
        }

        if(isMyPriorityInTheList)
        {
                int winnerProcessPriority = m_MyRank;
                //Stop election and see who's the winner
                for(it = priorityList.begin(); it < priorityList.end(); ++it)
                {
                        if(*it > winnerProcessPriority)
                        {
                                winnerProcessPriority = *it;
                        }
                }

                //Declare the winner to the other process
                m_OutLogFile << "Prioridade vencedora da eleição: " << winnerProcessPriority << std::endl;
                //TODO: spread message
                

        }
        else
        {
                int nextProcess = election_GetNextProcess(m_MyRank, &ignoreProcessList);

                //set own priority in the message
                sprintf(m_Message, "%s %d", m_Message, m_Priority);

                
                m_OutLogFile << "Mensagem construida: " << m_Message << std::endl;
                
                //Send election data to the next process
                sendMPIMessage(enmTagCommand__Master_StartElection, nextProcess);


        }



}

void SlaveProcess::sendMPIMessage(enmTagCommand commandTag, int destProcessId)
{
    m_OutLogFile << "SendMPI - Tag: " <<  commandTag <<  " - dest recebido: " << destProcessId << " - payload: " << m_Message << std::endl;
    MPI_Send(m_Message, strlen(m_Message) + 1, MPI_CHAR, destProcessId, commandTag, MPI_COMM_WORLD);
}

enmTagCommand SlaveProcess::receiveMPIMessage(int source)
{
        MPI_Status status; /* Return status*/
        MPI_Recv(m_Message, MESSAGE_SIZE, MPI_CHAR, source, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        m_OutLogFile << "Tag recebida: " << status.MPI_TAG << std::endl;

        return static_cast<enmTagCommand>(status.MPI_TAG);
}

void SlaveProcess::callCommand(enmTagCommand command)
{
        switch(command)
        {
                case enmTagCommand__KillProcess:
                        killProcess();
                break;

                case enmTagCommand__Master_StartElection:
                        masterStartElection();
                break;

                default:
                break;
        }
}

int SlaveProcess::election_GetNextProcess(int myRank, std::vector<int> *excludeList)
{
        int nextRank = _getNextRank(myRank);
        std::vector<int>::iterator it;

        for(it = excludeList->begin(); it < excludeList->end(); ++it)
        {
                if(*it == nextRank)
                {
                        nextRank = election_GetNextProcess(nextRank, excludeList);
                        break;
                }
        }

        return nextRank;
}