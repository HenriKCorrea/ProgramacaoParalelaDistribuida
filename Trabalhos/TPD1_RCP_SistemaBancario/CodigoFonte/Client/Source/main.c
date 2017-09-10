#include <stdio.h>
#include "CommandLineArgParser.h"
#include "commonDefinitions.h"
#include "Client.h"





int main(int argc, char** argv)
{
	operation_type command = INVALID_op;
	command = argParser_GetCommand(1, argv);
	  

	if (command != INVALID_op)
	{
		switch (command)
		{
			case CREATE_ACCOUNT:
			{
				Account newClientAccount;
				
				if (argc == 2) //Print to the user a form to get the necessary data.
				{
				}
				else //Extract command line optional arguments
				{
					unsigned int dataExtractedSuccessfully = 1; //true

					int nameArgIndex = 0;
					dataExtractedSuccessfully &= argParser_GetOpArgIndex(&nameArgIndex, argc, argv, NAME);

					int cpfArgIndex = 0;
					dataExtractedSuccessfully &= argParser_GetOpArgIndex(&cpfArgIndex, argc, argv, CPF);

					int balanceArgIndex = 0;
					dataExtractedSuccessfully &= argParser_GetOpArgIndex(&balanceArgIndex, argc, argv, BALANCE);

					int passwordArgIndex = 0;
					dataExtractedSuccessfully &= argParser_GetOpArgIndex(&passwordArgIndex, argc, argv, PASSWORD);

					if (dataExtractedSuccessfully == 1)
					{
						client_error_type opResult = client_SetNewClientAccount(&newClientAccount,
														argv[nameArgIndex + 1],
														argv[cpfArgIndex + 1],
														argv[balanceArgIndex + 1],
														argv[passwordArgIndex + 1]);
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

