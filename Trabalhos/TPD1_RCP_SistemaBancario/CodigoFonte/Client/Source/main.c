#include <stdio.h>
#include "CommandLineArgParser.h"

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
				//Declare account struct
				// account newClientAccount
				

				if (argv == 2) //Print to the user a form to get the necessary data.
				{
				}
				else //Extract command line optional arguments
				{
					unsigned int dataExtractedSuccessfully = 1; //true
					// dataExtractedSuccessfully &= argParser_GetOpArgIndex(argIndex, argc, argv, NAME);
					//if(dataExtractedSuccessfully) strcpy(newClientAccount.name, argv[argIndex], strlen

					// dataExtractedSuccessfully &= argParser_GetOpArgIndex(argIndex, argc, argv, CPF)
					// dataExtractedSuccessfully &= argParser_GetOpArgIndex(argIndex, argc, argv, BALANCE)
					// dataExtractedSuccessfully &= argParser_GetOpArgIndex(argIndex, argc, argv, PASSWORD)

					//setAccountField(account newClientAccount, NAME, char* argv, argvMinSize, argvMaxSize);
					//IsStringSizeInLimits(char* argv, argvMinSize, argvMaxSize)

					//TODO tentar criar biblioteca estatica com definição de estruturas globais.
					//Se não for bem sucedido, tentar incluir o .h como dependencia externa

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