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

static serverReturn_t clientReturn = {};


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

u_int * deposit_1_svc(balanceOperation_t *balanceData, struct svc_req *req)
{
	admin_error_t result = ADMIN_SUCCESS;	
	printf("Pedido recebido para deposito\n");

	//Check if CPF exists
	unsigned long tmpCPF;
	iCPFtol(&tmpCPF, balanceData->CPF);
	int accountIndex = getAccountIndex(accDatabase, accDatabase_Size, tmpCPF);
	if(accountIndex != -1)
	{
		//Deposit money
		accDatabase[accountIndex].balance += balanceData->balance;

		printf("Valor depositado com sucesso!\n");
	}
	else
	{
		result = ADMIN_DA_ACCNOTEXIST;
		printAdminError(result);
	}


	returnCode = result;
	return ((unsigned int *)&returnCode);	
}


serverReturn_t * withdraw_1_svc(balanceOperation_t *balanceData, struct svc_req *req)
{
	admin_error_t result = ADMIN_SUCCESS;	
	memset(&clientReturn, 0, sizeof(clientReturn));
	clientReturn.errorCode = ADMIN_SUCCESS;
	printf("Pedido recebido para saque\n");

	//Check if CPF exists
	unsigned long tmpCPF;
	iCPFtol(&tmpCPF, balanceData->CPF);
	int accountIndex = getAccountIndex(accDatabase, accDatabase_Size, tmpCPF);
	if(accountIndex != -1)
	{
		//Check if there's money available in account
		if(accDatabase[accountIndex].balance < balanceData->balance)
		{
			result = ADMIN_WITHDRAW_NOMONEY;
			printAdminError(result);	
		}
		else
		{
			//Withdraw money
			accDatabase[accountIndex].balance -= balanceData->balance;
			clientReturn.account = accDatabase[accountIndex];
			printf("Valor sacado com sucesso!\n");
			printAccount(&clientReturn.account);
		}
	}
	else
	{
		result = ADMIN_DA_ACCNOTEXIST;
		printAdminError(result);			
	}

	clientReturn.errorCode = result;
	return &clientReturn;	
}


serverReturn_t *authenticate_account_1_svc(RPC_CPF *rpcCPF, struct svc_req *req)
{
	admin_error_t result = ADMIN_SUCCESS;	
	memset(&clientReturn, 0, sizeof(clientReturn));
	clientReturn.errorCode = ADMIN_SUCCESS;	
	printf("Pedido recebido para autenticar conta\n");

	//Check if CPF exists
	unsigned long tmpCPF;
	iCPFtol(&tmpCPF, rpcCPF->CPF);
	int accountIndex = getAccountIndex(accDatabase, accDatabase_Size, tmpCPF);
	if(accountIndex != -1)
	{	
		clientReturn.account = accDatabase[accountIndex];
		printf("Conta autenticada com sucesso!\n");
		printAccount(&clientReturn.account);
	}
	else
	{
		result = ADMIN_DA_ACCNOTEXIST;
		printAdminError(result);			
	}


	clientReturn.errorCode = result;
	return &clientReturn;
}