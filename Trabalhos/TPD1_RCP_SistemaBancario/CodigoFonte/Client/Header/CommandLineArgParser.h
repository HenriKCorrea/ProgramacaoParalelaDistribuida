#ifndef CommandLineArgParser_h__
#define CommandLineArgParser_h__

//////////////////////////////////////////////////////////////////////////
//Global constants
//////////////////////////////////////////////////////////////////////////

//System operations available for the client
typedef enum operation_type {
	INVALID_op = 0,
	CREATE_ACCOUNT, 
	DELETE_ACCOUNT, 
	AUTHENTICATE_ACCOUNT,
	DEPOSIT,
	WITHDRAW} operation_type;

//Command names to identify console arguments
static const char* m_argParser_commandList[] = {
	"",
	"CRIARCONTA", 
	"EXCLUIRCONTA",
	"AUTENTICARCONTA",
	"DEPOSITAR",
	"RETIRAR"};

//number of command types
static const int numOfOperations = 5;

////Optional arguments described in enum form to be processed as flags.
//typedef enum optionalArgs_type {
//	INVALID_opAr = 0,
//	NAME = 1,
//	CPF = 2,
//	BALANCE = 4,
//	PASSWORD = 8,
//	ACCOUNT_ID = 16,
//	AGENCY_ID = 32} optionalArgs_type;

typedef enum optionalArgs_type {
	INVALID_opAr = 0,
	NAME,
	CPF,
	BALANCE,
	PASSWORD,
	ACCOUNT_ID,
	AGENCY_ID} optionalArgs_type;

static const int numOfOptionalArguments = 6;

static const char *m_argParser_optArgsShort[] = {
	"",
	"-n",
	"-c",
	"-s",
	"-p",
	"-nc",
	"-na"};

static const char *m_argParser_optArgsLong[] = {
	"",
	"--nome",
	"--cpf",
	"--saldo",
	"--senha",
	"--numero-conta",
	"--numero-agencia"};


/************************************************************************/
/* Public methods                                                       */
/************************************************************************/


/**
* @name		argParser_GetCommand
* @brief	Read "argv" in index "commandIndex" and, if is a valid 
*			command, extract its correspondent enum "operation_type"
*
* @pre		Command list "m_argParser_commandList" and enum
*			"operation_type" shall reflect the same declaration to each
*			command.
* @note		N/A
*
* @param[in] commandIndex	Index of command to be read in argv
* @param[in] argv			bits to be cleared on the specified port
*
* @return					The respective command in enum 
*							"operation_type"
* @retavl INVALID			If command is not located in 
*							"m_argParser_commandList"
*
* @author Henrique Krausburg Corrêa
*/
operation_type argParser_GetCommand(int commandIndex, char** argv);

unsigned int argParser_ExtractOptArgsFlag(int argc, char** argv);

unsigned int argParser_GetOpArgIndex(int* argIndex, int argc, char** argv, optionalArgs_type enmOptArg);

//////////////////////////////////////////////////////////////////////////
//Auxiliary public methods
//////////////////////////////////////////////////////////////////////////

//Convert "sourceString" to upper case and save in address "targetString
void stringToUpper(char* targetString, const char* sourceString, size_t size);

int extractStringDigits(char* targetString, const char* sourceString);

//////////////////////////////////////////////////////////////////////////
//Private methods
//////////////////////////////////////////////////////////////////////////




#endif // CommandLineArgParser_h__
