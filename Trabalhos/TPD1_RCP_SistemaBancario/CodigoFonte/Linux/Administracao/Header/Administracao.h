#ifndef ADMINISTRACAO_H__
#define ADMINISTRACAO_H__

#include "../../CommonDependencies/commonDefinitions.h"

/********************************/
/* private methods              */
/********************************/

unsigned char isCPFUnique(Account *accDB, int size, const unsigned long CPF);

#endif //ADMINISTRACAO_H__