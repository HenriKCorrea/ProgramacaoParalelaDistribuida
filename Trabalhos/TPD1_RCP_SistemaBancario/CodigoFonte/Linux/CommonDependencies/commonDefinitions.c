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

void printAccount(Account *acc)
{
    printf("Nome: %s\n", acc->name);
    printf("Conta: %d\n", acc->accountID);
    unsigned long int tmpCPF;
    iCPFtol(&tmpCPF, acc->CPF);
    printf("CPF: %ld\n", tmpCPF);
    printf("Saldo: %.2f\n", acc->balance);
}

void printAdminError(admin_error_t errorCode)
{
    switch(errorCode)
    {
        case ADMIN_CA_DATABASEFULL:
        fprintf(stderr, "Falha ao tentar criar conta: Banco de dados cheio!\n");
        break;

        case ADMIN_CA_CPFNOTEQUAL:
        fprintf(stderr, "Falha ao tentar criar conta: CPF ja existe no banco de dados\n");
        break;

        case ADMIN_DA_ACCNOTEXIST:
        fprintf(stderr, "Falha ao executar operacao deletar conta: Nenhum conta encontrada para o CPF informado.\n");
        break;

        case ADMIN_WITHDRAW_NOMONEY:
        fprintf(stderr, "Falha ao executar operacao Saque: Saldo insulficiente.\n");
        break;

        default:
        fprintf(stderr, "Falha ocorrida numero:%d\n", errorCode);
        break;
    }
}

void printClientError(client_error_type errorCode)
{
    switch(errorCode)
    {
        case CLIENT_NAME_SIZE_OVERFLOW:
        fprintf(stderr, "Campo 'Nome' invalido. Tamanho maximo: %d\n", ACC_NAME_LENGHT);
        break;

        case CLIENT_CPF_SIZE_OVERFLOW:
        case CLIENT_CPF_SIZE_UNDERFLOW:
        fprintf(stderr, "Campo 'CPF' invalido. Deve ser preenchido com 11 digitos\n");
        break;
        
        case CLIENT_BALANCE_DECIMAL_OVERFLOW:
        fprintf(stderr, "Campo 'Saldo' invalido. Use o formato xxx.xx\n");
        break;

        default:
        fprintf(stderr, "Falha ocorrida numero:%d\n", errorCode);
        break;
    }
}


