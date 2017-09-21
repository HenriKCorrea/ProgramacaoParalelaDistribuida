#ifndef commonDefinitions_h__
#define commonDefinitions_h__

#define ACC_NAME_LENGHT 30

#define ACC_CPF_DIGITS 11
#define ACC_CPF_DOT_DASHES 3


#include "RPCBank.h"

//typedef Account Account_RPC;

/***************************/
/* Account definitions     */
/***************************/

//typedef enum 


typedef struct Account_User
{
    unsigned int accountID;
	char name[30];
	unsigned long CPF;
	double balance;
}Account_User;

/***************************/
/* Public methods          */
/***************************/

void iCPFtol(unsigned long *lCPF, const unsigned int *iCPF);

void lCPFtoi(unsigned int *iCPF, const unsigned long *lCPF);


#endif // commonDefinitions_h__
