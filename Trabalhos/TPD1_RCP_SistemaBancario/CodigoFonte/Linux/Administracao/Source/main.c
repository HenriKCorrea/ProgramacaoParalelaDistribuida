#include <stdio.h>
#include <string.h>
#include <rpc/rpc.h>
#include "../../CommonDependencies/RPCBank.h"
#include "../Header/Administracao.h"

//#include "../../CommonDependencies/commonDefinitions.h"
#include "commonDefinitions.h"

#define ACCOUNT_DATABASE_LENGTH 30

//
Account accDatabase[ACCOUNT_DATABASE_LENGTH];
static unsigned int accDatabase_Size = 0;
static unsigned int accDatabase_NextID = 0;

static unsigned int returnCode = 0;


unsigned int *create_account_1_svc(Account *newAccount, struct svc_req *req) {
	admin_error_t result = ADMIN_SUCCESS;	
	printf("Pedido recebido para criar conta\n");

	//If database is not full, register account ID and store new client data.
	if(accDatabase_Size < ACCOUNT_DATABASE_LENGTH)
	{
		unsigned long tmpCPF;
		iCPFtol(&tmpCPF, newAccount->CPF);
		if(isCPFUnique(accDatabase, accDatabase_Size, tmpCPF))
		{
			++accDatabase_NextID;
			newAccount->accountID = accDatabase_NextID;
			accDatabase[accDatabase_Size] = *newAccount;
			++accDatabase_Size;
			printf("Conta criada com sucesso!\n");
		}
		else
		{
			result = ADMIN_CA_CPFNOTEQUAL;
			printAdminError(result);			
		}


	}
	else
	{
		result = ADMIN_CA_DATABASEFULL;
		printAdminError(result);
	}

	//Return error code
	returnCode = result;
	return ((unsigned int *) &returnCode);
}

u_int *delete_account_1_svc(RPC_CPF *rpcCPF, struct svc_req *req)
{
	admin_error_t result = ADMIN_SUCCESS;	
	printf("Pedido recebido para encerrar conta\n");

	//Check if CPF exists
	unsigned long tmpCPF;
	iCPFtol(&tmpCPF, rpcCPF->CPF);
	int accountIndex = getAccountIndex(accDatabase, accDatabase_Size, tmpCPF);
	if(accountIndex != -1)
	{
		//Delete account
		memset(&accDatabase[accountIndex], 0, sizeof(Account));
		sortDatabase(accDatabase, accDatabase_Size, accountIndex);
		--accDatabase_Size;
		printf("Conta deletada com sucesso!\n");
	}
	else
	{
		result = ADMIN_DA_ACCNOTEXIST;
		printAdminError(result);			
	}


	returnCode = result;
	return ((unsigned int *)&returnCode);
}

//TODO
//Criar metodo de deletar conta
//Criar metodo de depositar dinheiro na conta
//Criar metodo de sacar dinheiro da conta