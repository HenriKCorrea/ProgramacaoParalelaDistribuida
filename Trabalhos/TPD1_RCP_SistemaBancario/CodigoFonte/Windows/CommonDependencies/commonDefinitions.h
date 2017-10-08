#ifndef commonDefinitions_h__
#define commonDefinitions_h__

#define ACC_NAME_LENGHT 30

#define ACC_CPF_DIGITS 11
#define ACC_CPF_DOT_DASHES 3

#define  ACC_PASSWORD_LENGHT 6

typedef struct Account
{
	unsigned int accountID;
	unsigned int agencyID;
	char name[ACC_NAME_LENGHT];
	unsigned long CPF;
	double balance;
	unsigned int password;
}Account;



#endif // commonDefinitions_h__
