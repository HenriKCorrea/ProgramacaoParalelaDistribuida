#ifndef commonDefinitions_h__
#define commonDefinitions_h__

#define ACC_NAME_LENGHT 50

#define ACC_CPF_DIGITS 11
#define ACC_CPF_DOT_DASHES 3


#include "RPCBank.h"

//typedef Account Account_RPC;

/************************************************************************/
/* Global definitions                                                   */
/************************************************************************/

typedef enum client_error_type
{
	CLIENT_SUCCESS = 0,
	CLIENT_NAME_SIZE_OVERFLOW = 1,
	CLIENT_CPF_SIZE_OVERFLOW = 2,
	CLIENT_CPF_SIZE_UNDERFLOW = 3,
	CLIENT_BALANCE_DECIMAL_OVERFLOW = 4,
	CLIENT_PASSOWRD_SIZE_OVERFLOW = 5,
	CLIENT_PASSOWRD_INVALID_DIGIT = 6,
	CLIENT_ACCOUNTNUM_INVALID_DIGIT = 7,
	CLIENT_AGENCYNUM_INVALID_DIGIT = 8
}client_error_type;

typedef enum admin_error
{
	ADMIN_SUCCESS = 0,
	ADMIN_CA_DATABASEFULL = 1,
	ADMIN_CA_CPFNOTEQUAL = 2,
	ADMIN_DA_ACCNOTEXIST = 3
}admin_error_t;

typedef struct Account_User
{
    unsigned int accountID;
	char name[ACC_NAME_LENGHT];
	unsigned long CPF;
	double balance;
}Account_User;

/***************************/
/* Public methods          */
/***************************/

void iCPFtol(unsigned long *lCPF, const unsigned int *iCPF);

void lCPFtoi(unsigned int *iCPF, const unsigned long *lCPF);

void printAccount(Account *acc);

void printAdminError(admin_error_t errorCode);

void printClientError(client_error_type errorCode);


#endif // commonDefinitions_h__
