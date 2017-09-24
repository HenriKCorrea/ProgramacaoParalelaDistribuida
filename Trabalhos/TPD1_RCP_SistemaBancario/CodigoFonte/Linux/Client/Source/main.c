#include <stdio.h>
#include "../Header/CommandLineArgParser.h"
#include "../Header/Client.h"
#include "../../CommonDependencies/commonDefinitions.h"
#include "../../CommonDependencies/RPCBank.h"


int main(int argc, char** argv)
{
	operation_type command = OP_T_INVALID;
	command = argParser_GetCommand(1, argv);
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
						fprintf(stderr, "Uso:\tcriarConta <IP Processo Administrativo> -n <Nome> -c <CPF> -s <Saldo>\n");
					}
					
					free(newClientAccount);
				}
				break;

				case OP_T_DELETE_ACCOUNT:
				{
					//Required argument: CPF
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
						fprintf(stderr, "Uso:\tencerrarConta <IP Processo Administrativo> -c <CPF>\n");
					}					
				}
				break;


				default:
				{
					//fail
					fprintf(stderr, "falha ao processar %s [argumentos].\n", argv[1]);
					fprintf(stderr, "uso:\t%s <comando> <IP Processo Administrativo> [argumentos]\n\n", argv[0]);
					fprintf(stderr, "Comandos aceitos:\n\t");
					fprintf(stderr, "criarConta <IP Processo Administrativo> -n <Nome> -c <CPF> -s <Saldo>\n");
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
		fprintf(stderr, "criarConta <IP Processo Administrativo> -n <Nome> -c <CPF> -s <Saldo>\n");
	}


}

