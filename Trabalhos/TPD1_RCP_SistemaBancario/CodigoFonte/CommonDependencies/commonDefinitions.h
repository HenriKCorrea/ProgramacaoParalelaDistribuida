#ifndef commonDefinitions_h__
#define commonDefinitions_h__

typedef struct Account
{
	unsigned int accountID;
	unsigned int agencyID;
	char name[30];
	unsigned long CPF;
	double balance;
	unsigned int password;
}Account;

#endif // commonDefinitions_h__
