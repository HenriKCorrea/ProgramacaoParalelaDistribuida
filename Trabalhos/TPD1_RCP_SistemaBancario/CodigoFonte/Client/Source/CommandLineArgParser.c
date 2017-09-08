#include "CommandLineArgParser.h"

#include <string.h>
#include <ctype.h>


//////////////////////////////////////////////////////////////////////////
//Public methods
//////////////////////////////////////////////////////////////////////////

operation_type argParser_GetCommand(int commandIndex, char ** argv)
{
	operation_type resultCommand = INVALID; //command variable to be returned to the user.

	//Convert command to uppercase
	char argvCommandUpperCase[30] = {""};
	size_t argvSize = strlen(argv[commandIndex]);
	if (argvSize < 30)
	{
		stringToUpper(argvCommandUpperCase, argv[commandIndex], argvSize);

		//Loop over console arguments to verify if the first argument is a valid command
		for (int i = 1; i < numOfOperations + 1; ++i)
		{
			if (strcmp(argvCommandUpperCase, m_argParser_commandList[i]) == 0)
			{
				//If command match, set enum and exit loop.
				resultCommand = i;
				break;
			}
		}
	}

	return resultCommand;
}


//////////////////////////////////////////////////////////////////////////
//Auxiliary public methods
//////////////////////////////////////////////////////////////////////////

void stringToUpper(char* targetString, const char* sourceString, size_t size)
{
	for (int i = 0; i < size; ++i)
	{
		targetString[i] = toupper(sourceString[i]);
	}
}