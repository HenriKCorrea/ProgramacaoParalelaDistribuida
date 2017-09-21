#include <stdio.h>
#include <string.h>
#include <rpc/rpc.h>
#include "../../CommonDependencies/RPCBank.h"

//#include "../../CommonDependencies/commonDefinitions.h"
#include "commonDefinitions.h"

#define ACCOUNT_DATABASE_LENGTH 30

//
Account accDatabase[ACCOUNT_DATABASE_LENGTH];
static unsigned int accDatabase_Size = 0;
static unsigned int accDatabase_NextID = 0;


unsigned int *create_account_1_svc(Account *newAccount, struct svc_req *req) {

	//If database is not full, register account ID and store new client data.
	if(accDatabase_Size < ACCOUNT_DATABASE_LENGTH)
	{
		++accDatabase_Size;
		++accDatabase_NextID;
		newAccount->accountID = accDatabase_NextID;
		accDatabase[accDatabase_Size] = *newAccount;
	}

	//Return account ID
	return ((unsigned int *) &accDatabase_NextID);
}

//TODO
//Criar metodo de deletar conta
//Criar metodo de depositar dinheiro na conta
//Criar metodo de sacar dinheiro da conta