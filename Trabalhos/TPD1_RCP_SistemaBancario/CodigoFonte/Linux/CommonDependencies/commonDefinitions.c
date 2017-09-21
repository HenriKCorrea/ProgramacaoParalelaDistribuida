#include "commonDefinitions.h"


void iCPFtol(unsigned long *lCPF, const unsigned int *iCPF)
{
    *lCPF = iCPF[1] & 0x0ffffffff;
    *lCPF = (*lCPF << 32) | iCPF[0];
}

void lCPFtoi(unsigned int *iCPF, const unsigned long *lCPF)
{
    iCPF[1] = (*lCPF >> 32) & 0x0ffffffff;
    iCPF[0] = *lCPF & 0x0ffffffff ;
}
