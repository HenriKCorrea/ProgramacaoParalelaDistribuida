COM_DIR := CommonDependencies/
CLI_DIR := Client/Source/
ADM_DIR := Administracao/Source/


all: client server

RPCBank.h:
	cd CommonDependencies; rpcgen RPCBank.x

#RPCBank_svc.c RPCBank_clnt.c RPCBank_xdr.c add_cli.c add_svc.c: RPCBank.h

client: RPCBank_clnt.o RPCBank_xdr.o CommandLineArgParser.o Client.o main_client.o
	gcc -o client $(CLI_DIR)main.o

server: RPCBank_svc.o RPCBank_xdr.o
#	cc -o server add_svc.o RPCBank_svc.o RPCBank_xdr.o -lnsl
	

RPCBank_svc.o: RPCBank.h	
	gcc -g -c $(COM_DIR)RPCBank_svc.c -o $(COM_DIR)$@

RPCBank_clnt.o: RPCBank.h
	gcc -g -c $(COM_DIR)RPCBank_clnt.c -o $(COM_DIR)$@

RPCBank_xdr.o: RPCBank.h 
	gcc -g -c $(COM_DIR)RPCBank_xdr.c -o $(COM_DIR)$@

CommandLineArgParser.o:
	gcc -g -c $(CLI_DIR)CommandLineArgParser.c -o $(CLI_DIR)$@

Client.o: CommandLineArgParser.o RPCBank.h
	gcc -g -c $(CLI_DIR)Client.c -o $(CLI_DIR)$@

main_client.o: Client.o CommandLineArgParser.o
	gcc -g -c $(CLI_DIR)main.c -o $(CLI_DIR)main.o

#include "../Header/CommandLineArgParser.h"
#include "../Header/Client.h"
#include "../../CommonDependencies/commonDefinitions.h"
#include "../../CommonDependencies/RPCBank.h"


#add_cli.o: RPCBank.h 
#	gcc -g -c $(COM_DIR)add_cli.c -o $(COM_DIR)$@
#
#add_svc.o: RPCBank.h 
#	gcc -g -c add_svc.c

.PHONY: clean

clean:
	-rm client
	-rm *.o
	-rm $(COM_DIR)*.o
	-rm $(CLI_DIR)*.o
	-rm $(ADM_DIR)*.o
	-rm $(COM_DIR)RPCBank.h
	-rm $(COM_DIR)RPCBank_*

#Comandos CommonDependencies
#henrique@henrique-Inspiron-7520:~/Repositories/ProgramacaoParalelaDistribuida/Trabalhos/TPD1_RCP_SistemaBancario/CodigoFonte/Linux/CommonDependencies$ gcc -g -c RPCBank_xdr.c
#henrique@henrique-Inspiron-7520:~/Repositories/ProgramacaoParalelaDistribuida/Trabalhos/TPD1_RCP_SistemaBancario/CodigoFonte/Linux/CommonDependencies$ gcc -g -c RPCBank_svc.c
#henrique@henrique-Inspiron-7520:~/Repositories/ProgramacaoParalelaDistribuida/Trabalhos/TPD1_RCP_SistemaBancario/CodigoFonte/Linux/CommonDependencies$ gcc -g -c RPCBank_clnt.c

#Comandos Administracao
#henrique@henrique-Inspiron-7520:~/Repositories/ProgramacaoParalelaDistribuida/Trabalhos/TPD1_RCP_SistemaBancario/CodigoFonte/Linux/Administracao/Source$ gcc -g -c main.c
#henrique@henrique-Inspiron-7520:~/Repositories/ProgramacaoParalelaDistribuida/Trabalhos/TPD1_RCP_SistemaBancario/CodigoFonte/Linux/Administracao/Source$ gcc main.o ../../CommonDependencies/RPCBank_svc.o ../../CommonDependencies/RPCBank_xdr.o -o server

#Comandos Client
#henrique@henrique-Inspiron-7520:~/Repositories/ProgramacaoParalelaDistribuida/Trabalhos/TPD1_RCP_SistemaBancario/CodigoFonte/Linux/Client/Source$ gcc -g -c main.c
#henrique@henrique-Inspiron-7520:~/Repositories/ProgramacaoParalelaDistribuida/Trabalhos/TPD1_RCP_SistemaBancario/CodigoFonte/Linux/Client/Source$ gcc main.o CommandLineArgParser.o Client.o ../../CommonDependencies/RPCBank_clnt.o ../../CommonDependencies/RPCBank_xdr.o -o client
#henrique@henrique-Inspiron-7520:~/Repositories/ProgramacaoParalelaDistribuida/Trabalhos/TPD1_RCP_SistemaBancario/CodigoFonte/Linux/Client/Source$ gcc -g -c Client.chenrique@henrique-Inspiron-7520:~/Repositories/ProgramacaoParalelaDistribuida/Trabalhos/TPD1_RCP_SistemaBancario/CodigoFonte/Linux/Client/Source$ gcc -g -c CommandLineArgParser.c
#henrique@henrique-Inspiron-7520:~/Repositories/ProgramacaoParalelaDistribuida/Trabalhos/TPD1_RCP_SistemaBancario/CodigoFonte/Linux/Client/Source$ gcc -g -c main.chenrique@henrique-Inspiron-7520:~/Repositories/ProgramacaoParalelaDistribuida/Trabalhos/TPD1_RCP_SistemaBancario/CodigoFonte/Linux/Client/Source$ gcc main.o CommandLineArgParser.o Client.o ../../CommonDependencies/RPCBank_clnt.o ../../CommonDependencies/RPCBank_xdr.o -o client
