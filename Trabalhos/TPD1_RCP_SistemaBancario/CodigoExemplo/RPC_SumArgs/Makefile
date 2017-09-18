all: client server

addit.h: addit.x
	rpcgen addit.x

#addit_svc.c addit_clnt.c addit_xdr.c add_cli.c add_svc.c: addit.h

client: add_cli.o addit_clnt.o addit_xdr.o
	cc -o client add_cli.o addit_clnt.o addit_xdr.o -lnsl

server: add_svc.o addit_svc.o addit_xdr.o
	cc -o server add_svc.o addit_svc.o addit_xdr.o -lnsl

addit_svc.o: addit.h	
	gcc -g -c addit_svc.c

addit_clnt.o: addit.h
	gcc -g -c addit_clnt.c

addit_xdr.o: addit.h 
	gcc -g -c addit_xdr.c

add_cli.o: addit.h 
	gcc -g -c add_cli.c

add_svc.o: addit.h 
	gcc -g -c add_svc.c

.PHONY: clean

clean:
	-rm *.o
	-rm client
	-rm server
	-rm addit.h
	-rm addit_clnt.c
	-rm addit_svc.c
	-rm addit_xdr.c
