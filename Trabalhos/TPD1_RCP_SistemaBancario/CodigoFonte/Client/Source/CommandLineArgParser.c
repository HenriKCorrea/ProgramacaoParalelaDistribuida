#include "CommandLineArgParser.h"

#include <string.h>
#include <ctype.h>


//////////////////////////////////////////////////////////////////////////
//Public methods
//////////////////////////////////////////////////////////////////////////

//Read "argv" in index "commandIndex" and, if is a valid command, extract its correspondent enum "operation_type"
operation_type argParser_GetCommand(int commandIndex, char ** argv)
{
	operation_type resultCommand = INVALID_op; //command variable to be returned to the user.

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


unsigned int argParser_ExtractOptArgsFlag(int argc, char** argv)
{
	unsigned int resultOptArgsFlags = INVALID_opAr; //command variable to be returned to the user.

	for (int i = 1; i < argc + 1; ++i)
	{
		for (int j = 0; j < numOfOptionalArguments + 1; ++j)
		{
			if ((strcmp(argv[i], m_argParser_optArgsShort[j]) == 0) ||
				(strcmp(argv[i], m_argParser_optArgsLong[j]) == 0))
			{
				//If command match, set enum and exit loop.
				resultOptArgsFlags |= (1 << j);
				break;
			}
		}

	}

	return 0;
}

//Get element index
//f

int getEnumTableIndex(optionalArgs_type enumOptArg)
{
	unsigned int enumValue = enumOptArg;
	int i = 0;
	for (i; enumValue != 0; ++i)
	{
		enumValue >> 1;
	}
	return i;
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