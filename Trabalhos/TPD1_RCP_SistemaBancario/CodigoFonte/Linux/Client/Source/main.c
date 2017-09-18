#include <stdio.h>
#include "../Header/CommandLineArgParser.h"
#include "../Header/Client.h"
#include "../../CommonDependencies/commonDefinitions.h"
#include "../../CommonDependencies/RPCBank.h"



int main(int argc, char** argv)
{
	operation_type command = OP_T_INVALID;
	command = argParser_GetCommand(1, argv);
	  

	if (command != OP_T_INVALID)
	{
		switch (command)
		{
			case OP_T_CREATE_ACCOUNT:
			{
				//Account *newClientAccount = (Account *) malloc(sizeof(Account));
				Account newClientAccount;
				
				if (argc == 2) //Print to the user a form to get the necessary data.
				{
				}
				else //Extract command line optional arguments
				{
					unsigned int dataExtractedSuccessfully = 1; //true

					int nameArgIndex = 0;
					dataExtractedSuccessfully &= argParser_GetOpArgIndex(&nameArgIndex, argc, argv, OP_AR_NAME);

					int cpfArgIndex = 0;
					dataExtractedSuccessfully &= argParser_GetOpArgIndex(&cpfArgIndex, argc, argv, OP_AR_CPF);

					int balanceArgIndex = 0;
					dataExtractedSuccessfully &= argParser_GetOpArgIndex(&balanceArgIndex, argc, argv, OP_AR_BALANCE);

					if (dataExtractedSuccessfully == 1)
					{
						client_error_type opResult = client_SetNewClientAccount(&newClientAccount,
														argv[nameArgIndex + 1],
														argv[cpfArgIndex + 1],
														argv[balanceArgIndex + 1]);


						//TODO chamar RPC
						CLIENT *cl;
						if (!(cl = clnt_create(argv[2], RPCBANK_PROGRAM, RPCBANK_VERS,"tcp"))) {
							clnt_pcreateerror(argv[2]);
							exit(1);
							}
						unsigned int answer = *create_account_1(&newClientAccount,cl);
						printf("Resultado criar conta: %d\n", answer);
						
					}

					//if(dataExtractedSuccessfully) strcpy(newClientAccount.name, argv[argIndex], strlen

					// dataExtractedSuccessfully &= argParser_GetOpArgIndex(argIndex, argc, argv, CPF)
					// dataExtractedSuccessfully &= argParser_GetOpArgIndex(argIndex, argc, argv, BALANCE)
					// dataExtractedSuccessfully &= argParser_GetOpArgIndex(argIndex, argc, argv, PASSWORD)

					//setAccountField(account newClientAccount, NAME, char* argv, argvMinSize, argvMaxSize);
					//IsStringSizeInLimits(char* argv, argvMinSize, argvMaxSize)


				}
				
			}
			break;

			default:
			//fail
			break;
		}
	} 
	else
	{
		//fail
	}


}

