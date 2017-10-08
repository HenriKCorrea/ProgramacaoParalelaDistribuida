COM_DIR := CommonDependencies/
CLI_DIR := Client/Source/
ADM_DIR := Administracao/Source/


all: client server

RPCBank.h:
	cd CommonDependencies; rpcgen RPCBank.x

#RPCBank_svc.c RPCBank_clnt.c RPCBank_xdr.c add_cli.c add_svc.c: RPCBank.h

client: RPCBank_clnt.o RPCBank_xdr.o CommandLineArgParser.o Client.o main_client.o commonDefinitions.o
	gcc -o client $(CLI_DIR)main.o $(CLI_DIR)Client.o $(CLI_DIR)CommandLineArgParser.o $(COM_DIR)RPCBank_xdr.o $(COM_DIR)RPCBank_clnt.o  $(COM_DIR)commonDefinitions.o

server: RPCBank_svc.o RPCBank_xdr.o main_server.o commonDefinitions.o Administracao.o
	gcc -o server $(ADM_DIR)main.o $(ADM_DIR)Administracao.o $(COM_DIR)RPCBank_xdr.o $(COM_DIR)RPCBank_svc.o $(COM_DIR)commonDefinitions.o
	
RPCBank_svc.o: RPCBank.h	
	gcc -g -O0 -c $(COM_DIR)RPCBank_svc.c -o $(COM_DIR)$@

RPCBank_clnt.o: RPCBank.h
	gcc -g -O0 -c $(COM_DIR)RPCBank_clnt.c -o $(COM_DIR)$@

RPCBank_xdr.o: RPCBank.h 
	gcc -g -O0 -c $(COM_DIR)RPCBank_xdr.c -o $(COM_DIR)$@

CommandLineArgParser.o:
	gcc -g -O0 -c $(CLI_DIR)CommandLineArgParser.c -o $(CLI_DIR)$@

Client.o: CommandLineArgParser.o RPCBank.h
	gcc -g -O0 -c $(CLI_DIR)Client.c -o $(CLI_DIR)$@

main_client.o: Client.o CommandLineArgParser.o
	gcc -g -O0 -c $(CLI_DIR)main.c -o $(CLI_DIR)main.o

main_server.o: RPCBank.h
	gcc -g -O0 -c $(ADM_DIR)main.c -I$(COM_DIR) -o $(ADM_DIR)main.o	

commonDefinitions.o:
	gcc -g -O0 -c $(COM_DIR)commonDefinitions.c -o $(COM_DIR)$@

Administracao.o: commonDefinitions.o
	gcc -g -O0 -c $(ADM_DIR)Administracao.c -o $(ADM_DIR)$@



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
	-rm server
	-rm $(COM_DIR)*.o
	-rm $(CLI_DIR)*.o
	-rm $(ADM_DIR)*.o
	-rm $(COM_DIR)RPCBank.h
	-rm $(COM_DIR)RPCBank_*
