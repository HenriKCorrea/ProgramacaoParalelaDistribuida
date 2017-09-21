/* this code will be translated into the needed stubs and headers */
/* use: rpcgen RPCBank.x; */



struct Account
{
	unsigned int accountID;
	char name[30];
	unsigned int CPF[2];		/*Necessary an array because RPC do not support unsigned long 64 bits type */
	double balance;
};

/* arguments for RPC must be one single */
/* value or a structure of values */
/* first_num and second_num are addends */
program RPCBANK_PROGRAM {             /*value to register the program */
version RPCBANK_VERS {             /*version must be assigned a value */
unsigned int CREATE_ACCOUNT(Account) = 1;       /*this is the service function */
} =1;                           /*version value */
} = 0x20000001;                 /*program value */
