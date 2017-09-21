#include "../Header/CommandLineArgParser.h"

#include <string.h>
#include <ctype.h>


//////////////////////////////////////////////////////////////////////////
//Public methods
//////////////////////////////////////////////////////////////////////////

//Read "argv" in index "commandIndex" and, if is a valid command, extract its correspondent enum "operation_type"
operation_type argParser_GetCommand(int commandIndex, char ** argv)
{
	operation_type resultCommand = OP_T_INVALID; //command variable to be returned to the user.

	//Convert command to uppercase
	char argvCommandUpperCase[50] = {""};
	size_t argvSize = strlen(argv[commandIndex]);
	if (argvSize < 50)
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


//unsigned int argParser_ExtractOptArgsFlag(int argc, char** argv)
//{
//	unsigned int resultOptArgsFlags = INVALID_opAr; //command variable to be returned to the user.
//
//	for (int i = 1; i < argc + 1; ++i)
//	{
//		for (int j = 0; j < numOfOptionalArguments + 1; ++j)
//		{
//			if ((strcmp(argv[i], m_argParser_optArgsShort[j]) == 0) ||
//				(strcmp(argv[i], m_argParser_optArgsLong[j]) == 0))
//			{
//				//If command match, set enum and exit loop.
//				resultOptArgsFlags |= (1 << j);
//				break;
//			}
//		}
//
//	}
//
//	return 0;
//}

//Get element index
//f

//int getEnumTableIndex(optionalArgs_type enumOptArg)
//{
//	unsigned int enumValue = enumOptArg;
//	int i = 0;
//	for (i; enumValue != 0; ++i)
//	{
//		enumValue = 1;
//	}
//	return i;
//}

unsigned int argParser_GetOpArgIndex(int* argIndex, int argc, char** argv, optionalArgs_type enmOptArg)
{
	unsigned int operationResult = 0; //false
	int optArgTableIndex = enmOptArg;
	for (int i = 1; i < argc; ++i)
	{
		if ((strcmp(argv[i], m_argParser_optArgsShort[optArgTableIndex]) == 0) ||
			(strcmp(argv[i], m_argParser_optArgsLong[optArgTableIndex]) == 0))
		{
			//If command match, set index, set flag to true and break loop
			*argIndex = i;
			operationResult = 1;
			break;
		}
	}
	return operationResult;
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

int extractStringDigits(char* targetString, const char* sourceString)
{
	size_t sourceSize = strlen(sourceString);
	int targetIndex = 0;
	for (int i = 0; i < sourceSize; ++i)
	{
		if (isdigit(sourceString[i]))
		{
			if (targetString != NULL)
			{
				targetString[targetIndex] = sourceString[i];
			}
			
			++targetIndex;
		}
	}
	return targetIndex;
}