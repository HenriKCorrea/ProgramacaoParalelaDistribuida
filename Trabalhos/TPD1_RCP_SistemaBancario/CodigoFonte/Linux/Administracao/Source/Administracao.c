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

