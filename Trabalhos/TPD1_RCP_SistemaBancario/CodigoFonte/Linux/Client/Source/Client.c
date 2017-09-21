#include "../Header/Client.h"

#include "../Header/CommandLineArgParser.h"
#include "../../CommonDependencies/RPCBank.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/************************************************************************/
/* Public methods														*/
/************************************************************************/

client_error_type client_SetNewClientAccount(Account* clientAccount, const char* cName, const char* cCPF, const char* cBalance)
{
	client_error_type opResult = CLIENT_SUCCESS;
	int i = 0;
	do 
	{
		switch (i)
		{
		case 0:
		{
			opResult = client_SetName(clientAccount, cName);
			break;
		}
		case 1:
		{
			opResult = client_SetCPF(clientAccount, cCPF);
			break;
		}
		case 2:
		{
			opResult = client_SetBalance(clientAccount, cBalance);
			break;
		}
		default:
			//FAIL
			break;
		}
		++i;
	} while ((opResult == CLIENT_SUCCESS) && (i < 3));

	return opResult;
}

client_error_type client_SetName(Account* clientAccount, const char* argv)
{
	client_error_type opResult = CLIENT_SUCCESS;
	size_t argv_len = strlen(argv);
	if (argv_len > ACC_NAME_LENGHT)
	{
		opResult = CLIENT_NAME_SIZE_OVERFLOW;
	}
	else
	{
		//newClientAccount->name
		strcpy(clientAccount->name, argv);
	}

	return opResult;
}

client_error_type client_SetCPF(Account* clientAccount, const char* argv)
{
	client_error_type opResult = CLIENT_SUCCESS;
	size_t argv_len = strlen(argv);

	if (argv_len > (ACC_CPF_DIGITS + ACC_CPF_DOT_DASHES))
	{
		opResult = CLIENT_CPF_SIZE_OVERFLOW;
	}
	else
	{
		char tmpCPFBuffer[ACC_CPF_DIGITS + ACC_CPF_DOT_DASHES] = "";
		int numOfDigitsExtracted = extractStringDigits(tmpCPFBuffer, argv);
		if (numOfDigitsExtracted < ACC_CPF_DIGITS)
		{
			opResult = CLIENT_CPF_SIZE_UNDERFLOW;
		} 
		else
		{
			unsigned long tmpCPF = strtoul(tmpCPFBuffer, NULL, 10);
			lCPFtoi(clientAccount->CPF, &tmpCPF);
		}
	}
	return opResult;
}



client_error_type client_SetBalance(Account* clientAccount, const char* argv)
{
	client_error_type opResult = CLIENT_SUCCESS;

	double extractedBalance = strtod(argv, NULL);
	if (isBalanceValid(extractedBalance) == 0)
	{
		opResult = CLIENT_BALANCE_DECIMAL_OVERFLOW;
	} 
	else
	{
		clientAccount->balance = extractedBalance;
	}

	return opResult;
}


client_error_type client_SetAccountNumber(Account* clientAccount, const char* argv)
{
	client_error_type opResult = CLIENT_SUCCESS;

	int numOfDigitsExtracted = extractStringDigits(NULL, argv);
	size_t argv_len = strlen(argv);
	if (numOfDigitsExtracted != argv_len)
	{
		opResult = CLIENT_ACCOUNTNUM_INVALID_DIGIT;
	}
	else
	{
		clientAccount->accountID = strtoul(argv, NULL, 10);
	}

	return opResult;
}

int createRPCConnection(CLIENT **client, char* serverIP)
{
	int result = 1;
	if (!(*client = clnt_create(serverIP, RPCBANK_PROGRAM, RPCBANK_VERS,"tcp"))) {
		result = 0;
	}
	return result;
}

//TO IMPLEMENT
client_error_type client_ExtractCPF(Account* clientAccount, const char* argv)
{
	client_error_type opResult = CLIENT_SUCCESS;
	// size_t argv_len = strlen(argv);

	// if (argv_len > (ACC_CPF_DIGITS + ACC_CPF_DOT_DASHES))
	// {
	// 	opResult = CLIENT_CPF_SIZE_OVERFLOW;
	// }
	// else
	// {
	// 	char tmpCPFBuffer[ACC_CPF_DIGITS + ACC_CPF_DOT_DASHES] = "";
	// 	int numOfDigitsExtracted = extractStringDigits(tmpCPFBuffer, argv);
	// 	if (numOfDigitsExtracted < ACC_CPF_DIGITS)
	// 	{
	// 		opResult = CLIENT_CPF_SIZE_UNDERFLOW;
	// 	} 
	// 	else
	// 	{
	// 		unsigned long tmpCPF = strtoul(tmpCPFBuffer, NULL, 10);
	// 		lCPFtoi(clientAccount->CPF, &tmpCPF);
	// 	}
	// }
	return opResult;
}



/************************************************************************/
/* Auxiliary public methods                                             */
/************************************************************************/

int isBalanceValid(double balanceValue)
{
	int result = 1;
	if(balanceValue < 0)
	{
		//negative value
		result = 0;
	}
	else
	{
		double dummyValue;
		double remainingDecimal = modf(balanceValue * 100, &dummyValue);
	
		if (remainingDecimal > 1e-10 /* Maximum tolerance */)
		{
			//Balance is invalid, have more than two decimals digits.
			result = 0;
		}		
	}	
	
	return result;
}