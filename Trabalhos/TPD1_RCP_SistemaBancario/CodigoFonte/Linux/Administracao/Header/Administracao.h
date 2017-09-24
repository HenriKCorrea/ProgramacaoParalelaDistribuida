#ifndef ADMINISTRACAO_H__
#define ADMINISTRACAO_H__

#include "../../CommonDependencies/commonDefinitions.h"

/********************************/
/* private methods              */
/********************************/

unsigned char isCPFUnique(Account *accDB, int size, const unsigned long CPF);

int getAccountIndex(Account *accDB, int size, const unsigned long CPF);

void sortDatabase(Account *accDB, int size, int offset);

#endif //ADMINISTRACAO_H__