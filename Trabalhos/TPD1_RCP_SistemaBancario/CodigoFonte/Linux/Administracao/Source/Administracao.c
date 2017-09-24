#include "../Header/Administracao.h"

unsigned char isCPFUnique(Account *accDB, int size, const unsigned long CPF)
{
    unsigned char result = 1; //TRUE

    unsigned long tmpCPF;
    for (int i = 0; i < size; ++i)
    {
        iCPFtol(&tmpCPF, accDB[i].CPF);
        if(tmpCPF == CPF)
        {
            result = 0;
            break;
        }
    }

    return result;
}

int getAccountIndex(Account *accDB, int size, const unsigned long CPF)
{
    int result = -1; //FALSE
    
        unsigned long tmpCPF;
        for (int i = 0; i < size; ++i)
        {
            iCPFtol(&tmpCPF, accDB[i].CPF);
            if(tmpCPF == CPF)
            {
                result = i;
                break;
            }
        }
    
        return result;    
}

void sortDatabase(Account *accDB, int size, int offset)
{
    for(int i = offset; i < size; ++i)
    {
        memcpy(&accDB[i], &accDB[i + 1], sizeof(Account));
    }
}