#ifndef GLOBALDEFINITIONS__H
#define GLOBALDEFINITIONS__H

#define MESSAGE_SIZE 100

enum enmTagCommand 
{
    enmTagCommand__KillProcess = 0,
    enmTagCommand__Master_StartElection,
    enmTagCommand__Slave_Election,
    enmTagCommand__Master_SetPriority,
    enmTagCommand__Slave_SetPriority,
    
    enmTagCommand__EndOfList
};



#endif //GLOBALDEFINITIONS__H