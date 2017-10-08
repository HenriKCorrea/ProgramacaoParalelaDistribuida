#ifndef Client_h__
#define Client_h__

#include "../../CommonDependencies/commonDefinitions.h"
#include "../../CommonDependencies/RPCBank.h"
#include "CommandLineArgParser.h"



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

int createRPCConnection(CLIENT **client, char *serverIP);

client_error_type client_ExtractCPF(unsigned long *CPF, const char* argv);

client_error_type client_ExtractValue(double *value, const char* argv);

/************************************************************************/
/* Auxiliary public methods                                             */
/************************************************************************/

int isBalanceValid(double balanceValue);

#endif // Client_h__
