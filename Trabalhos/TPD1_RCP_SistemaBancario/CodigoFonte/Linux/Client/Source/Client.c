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


client_error_type client_ExtractCPF(unsigned long *CPF, const char* argv)
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
			*CPF = strtoul(tmpCPFBuffer, NULL, 10);
			
		}
	}
	return opResult;
}

client_error_type client_ExtractValue(double *value, const char* argv)
{
	client_error_type opResult = CLIENT_SUCCESS;

	double extractedBalance = strtod(argv, NULL);
	if (isBalanceValid(extractedBalance) == 0)
	{
		opResult = CLIENT_BALANCE_DECIMAL_OVERFLOW;
	} 
	else
	{
		*value= extractedBalance;
	}

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

/************************************************************************/
/* Main methods														    */
/************************************************************************/

int client_main(int argc, char** argv)
{
	operation_type command = OP_T_INVALID;

	command = argParser_GetCommand(1, argv, argc);
	if ((command != OP_T_INVALID) && argc > 2)
	{
		//Create connetction with server
		CLIENT *cl;
		if(createRPCConnection(&cl, argv[2]))
		{
			switch (command)
			{
			case OP_T_CREATE_ACCOUNT:
			{
				Account *newClientAccount = (Account *) malloc(sizeof(Account));
				
				unsigned int dataExtractedSuccessfully = 1; //true

				int nameArgIndex = 0;
				dataExtractedSuccessfully &= argParser_GetOpArgIndex(&nameArgIndex, argc, argv, OP_AR_NAME);

				int cpfArgIndex = 0;
				dataExtractedSuccessfully &= argParser_GetOpArgIndex(&cpfArgIndex, argc, argv, OP_AR_CPF);

				int balanceArgIndex = 0;
				dataExtractedSuccessfully &= argParser_GetOpArgIndex(&balanceArgIndex, argc, argv, OP_AR_BALANCE);

				if (dataExtractedSuccessfully == 1)
				{
					client_error_type opResult = client_SetNewClientAccount(newClientAccount,
													argv[nameArgIndex + 1],
													argv[cpfArgIndex + 1],
													argv[balanceArgIndex + 1]);
					
					if(opResult == CLIENT_SUCCESS)
					{
						admin_error_t answer= *create_account_1(newClientAccount,cl);
						if(answer == ADMIN_SUCCESS)
						{
							printf("Operação realizada com sucesso!\n");
						}
						else
						{
							printAdminError(answer);
						}
						
					}
					else
					{
						fprintf(stderr, "Falha ao processar argumentos do comando %s: ", argv[1]);
						printClientError(opResult);
					}
				}
				else
				{
					fprintf(stderr, "Falha ao extrair argumentos do comando %s.\n", argv[1]);
				}
				
				free(newClientAccount);
			}
			break;

			case OP_T_DEPOSIT:
			{
				//Required argument: CPF
				unsigned int dataExtractedSuccessfully = 1; //true
				
				//Get CPF arg ID
				int cpfArgIndex = 0;
				dataExtractedSuccessfully &= argParser_GetOpArgIndex(&cpfArgIndex, argc, argv, OP_AR_CPF);

				int valueArgIndex = 0;
				dataExtractedSuccessfully &= argParser_GetOpArgIndex(&valueArgIndex, argc, argv, OP_AR_BALANCE);					
				
				//Check if argument flag has been detected
				if (dataExtractedSuccessfully == 1)
				{
					//Get CPF data
					balanceOperation_t balanceOpData;
					unsigned long CPF = 0;
					client_error_type opResult = client_ExtractCPF(&CPF, argv[cpfArgIndex + 1]);
					lCPFtoi(balanceOpData.CPF, &CPF);

					//Get ballance data
					client_ExtractValue(&balanceOpData.balance, argv[valueArgIndex + 1]);

					if(opResult == CLIENT_SUCCESS)
					{
						//Send command to server
						admin_error_t answer= *deposit_1(&balanceOpData,cl);	
						if(answer == ADMIN_SUCCESS)
						{
							printf("Operação realizada com sucesso!\n");
						}
						else
						{
							printAdminError(answer);
						}
						
					}
					else
					{
						fprintf(stderr, "Falha ao processar argumentos do comando %s: ", argv[1]);
						printClientError(opResult);
					}
				}
				else
				{
					fprintf(stderr, "Falha ao extrair argumentos do comando %s.\n", argv[1]);
				}					
			}
			break;


			case OP_T_WITHDRAW:
			{
				//Required argument: CPF
				unsigned int dataExtractedSuccessfully = 1; //true
				
				//Get CPF arg ID
				int cpfArgIndex = 0;
				dataExtractedSuccessfully &= argParser_GetOpArgIndex(&cpfArgIndex, argc, argv, OP_AR_CPF);

				int valueArgIndex = 0;
				dataExtractedSuccessfully &= argParser_GetOpArgIndex(&valueArgIndex, argc, argv, OP_AR_BALANCE);					
				
				//Check if argument flag has been detected
				if (dataExtractedSuccessfully == 1)
				{
					//Get CPF data
					balanceOperation_t balanceOpData;
					unsigned long CPF = 0;
					client_error_type opResult = client_ExtractCPF(&CPF, argv[cpfArgIndex + 1]);
					lCPFtoi(balanceOpData.CPF, &CPF);

					//Get ballance data
					client_ExtractValue(&balanceOpData.balance, argv[valueArgIndex + 1]);

					if(opResult == CLIENT_SUCCESS)
					{
						//Send command to server
						serverReturn_t serverAnswer = *withdraw_1(&balanceOpData,cl);	
						if(serverAnswer.errorCode == ADMIN_SUCCESS)
						{
							printf("Operação realizada com sucesso!\n");
							printAccount(&serverAnswer.account);
						}
						else
						{
							printAdminError(serverAnswer.errorCode);
						}
						
					}
					else
					{
						fprintf(stderr, "Falha ao processar argumentos do comando %s: ", argv[1]);
						printClientError(opResult);
					}
				}
				else
				{
					fprintf(stderr, "Falha ao extrair argumentos do comando %s.\n", argv[1]);
				}					
			}
			break;		
			
			case OP_T_AUTHENTICATE_ACCOUNT:
			{
				//Required argument: CPF
				unsigned int dataExtractedSuccessfully = 1; //true
				
				//Get CPF arg ID
				int cpfArgIndex = 0;
				dataExtractedSuccessfully &= argParser_GetOpArgIndex(&cpfArgIndex, argc, argv, OP_AR_CPF);
				
				//Check if argument flag has been detected
				if (dataExtractedSuccessfully == 1)
				{
					//Get CPF data
					unsigned long CPF = 0;
					client_error_type opResult = client_ExtractCPF(&CPF, argv[cpfArgIndex + 1]);

					if(opResult == CLIENT_SUCCESS)
					{
						RPC_CPF outData;
						lCPFtoi(outData.CPF, &CPF);

						//Send command to server
						serverReturn_t serverAnswer = *authenticate_account_1(&outData,cl);	
						if(serverAnswer.errorCode == ADMIN_SUCCESS)
						{
							printf("Operação realizada com sucesso!\n");
							printAccount(&serverAnswer.account);
						}
						else
						{
							printAdminError(serverAnswer.errorCode);
						}
						
					}
					else
					{
						fprintf(stderr, "Falha ao processar argumentos do comando %s: ", argv[1]);
						printClientError(opResult);
					}
				}
				else
				{
					fprintf(stderr, "Falha ao extrair argumentos do comando %s.\n", argv[1]);
				}					
			}
			break;			
			

			case OP_T_DELETE_ACCOUNT:
			{
				unsigned int dataExtractedSuccessfully = 1; //true
				
				//Get CPF arg ID
				int cpfArgIndex = 0;
				dataExtractedSuccessfully &= argParser_GetOpArgIndex(&cpfArgIndex, argc, argv, OP_AR_CPF);
				
				//Check if argument flag has been detected
				if (dataExtractedSuccessfully == 1)
				{
					//Set CPF
					unsigned long CPF = 0;
					client_error_type opResult = client_ExtractCPF(&CPF, argv[cpfArgIndex + 1]);
					if(opResult == CLIENT_SUCCESS)
					{
						//Send command to server
						RPC_CPF outData;
						lCPFtoi(outData.CPF, &CPF);
						admin_error_t answer= *delete_account_1(&outData,cl);	
						if(answer == ADMIN_SUCCESS)
						{
							printf("Operação realizada com sucesso!\n");
						}
						else
						{
							printAdminError(answer);
						}
						
					}
					else
					{
						fprintf(stderr, "Falha ao processar argumentos do comando %s: ", argv[1]);
						printClientError(opResult);
					}
				}
				else
				{
					fprintf(stderr, "Falha ao extrair argumentos do comando %s.\n", argv[1]);
				}					
			}
			break;


			default:
			{
				//fail
				fprintf(stderr, "falha ao processar %s [argumentos].\n", argv[1]);
				fprintf(stderr, "uso:\t%s <comando> <IP Processo Administrativo> [argumentos]\n\n", argv[0]);
				fprintf(stderr, "Comandos aceitos:\n\t");
				fprintf(stderr, "criarConta <IP Processo Administrativo> -n <Nome> -c <CPF> -s <Saldo>\n\t");
				fprintf(stderr, "encerrarConta <IP Processo Administrativo> -c <CPF>\n\t");
				fprintf(stderr, "depositar <IP Processo Administrativo> -c <CPF> -s <Saldo>\n\t");
				fprintf(stderr, "retirar <IP Processo Administrativo> -c <CPF> -s <Saldo>\n\t");
				fprintf(stderr, "autenticarConta <IP Processo Administrativo> -c <CPF>\n");
			}
			break;
			}			
		}
		else
		{
			//Fail: unable to stablish connection with server
			fprintf(stderr, "falha ao tentar se comunicar com o servidor: "); 
			clnt_pcreateerror(argv[2]);
		}
	} 
	else
	{
		//Fail: invalid command
		fprintf(stderr, "falha ao processar <comando>.\n");
		fprintf(stderr, "uso:\t%s <comando> <IP Processo Administrativo> [argumentos]\n\n", argv[0]);
		fprintf(stderr, "Comandos aceitos:\n\t");
		fprintf(stderr, "criarConta <IP Processo Administrativo> -n <Nome> -c <CPF> -s <Saldo>\n\t");
		fprintf(stderr, "encerrarConta <IP Processo Administrativo> -c <CPF>\n");
		fprintf(stderr, "depositar <IP Processo Administrativo> -c <CPF> -s <Saldo>\n\t");
		fprintf(stderr, "retirar <IP Processo Administrativo> -c <CPF> -s <Saldo>\n\t");
		fprintf(stderr, "autenticarConta <IP Processo Administrativo> -c <CPF>\n");
	}


}

