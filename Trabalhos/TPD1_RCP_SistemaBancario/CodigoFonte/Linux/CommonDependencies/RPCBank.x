/* this code will be translated into the needed stubs and headers */
/* use: rpcgen RPCBank.x; */



struct Account
{
	unsigned int accountID;
	char name[50];
	unsigned int CPF[2];		/*Necessary an array because RPC do not support unsigned long 64 bits type */
	double balance;
};

struct AccountOperation_t
{
	Account account;
	unsigned int operationTag;
};

struct RPC_CPF
{
	unsigned int CPF[2];
	unsigned int operationTag;
};

struct balanceOperation_t
{
	unsigned int CPF[2];
	double balance;
	unsigned int operationTag;
};

struct serverReturn_t
{
	Account account;
	unsigned int errorCode;
};



/* arguments for RPC must be one single */
/* value or a structure of values */
/* first_num and second_num are addends */
program RPCBANK_PROGRAM {             /*value to register the program */
version RPCBANK_VERS {             /*version must be assigned a value */
unsigned int CREATE_ACCOUNT(Account) = 1;       /*this is the service function */
unsigned int DELETE_ACCOUNT(RPC_CPF) = 2;
unsigned int DEPOSIT(balanceOperation_t) = 3;
serverReturn_t WITHDRAW(balanceOperation_t) = 4;
serverReturn_t AUTHENTICATE_ACCOUNT(RPC_CPF) = 5;
} =1;                           /*version value */
} = 0x20000001;                 /*program value */
