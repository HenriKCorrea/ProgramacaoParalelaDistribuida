#ifndef Client_h__
#define Client_h__

#include "../../CommonDependencies/commonDefinitions.h"
#include "../../CommonDependencies/RPCBank.h"
#include "CommandLineArgParser.h"

/************************************************************************/
/* Global variables                                                     */
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

/************************************************************************/
/* Public methods														*/
/************************************************************************/

client_error_type client_SetNewClientAccount(Account* clientAccount, const char* cName, const char* cCPF, const char* cBalance);

client_error_type client_SetName(Account* clientAccount, const char* argv);

client_error_type client_SetCPF(Account* clientAccount, const char* argv);

client_error_type client_SetBalance(Account* clientAccount, const char* argv);

client_error_type client_SetPassword(Account* clientAccount, const char* argv);

client_error_type client_SetAccountNumber(Account* clientAccount, const char* argv);

client_error_type client_SetAgencyNumber(Account* clientAccount, const char* argv);


//TO DELETE
client_error_type client_SetCPFTEST(Account_User* clientAccount, const char* argv);

/************************************************************************/
/* Auxiliary public methods                                             */
/************************************************************************/

int isBalanceValid(double balanceValue);

#endif // Client_h__
