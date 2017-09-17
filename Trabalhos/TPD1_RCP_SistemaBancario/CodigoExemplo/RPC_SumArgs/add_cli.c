/* addit.c client code for the example application */
#include <stdio.h>
#include <rpc/rpc.h>
#include "addit.h"
int main(int argc, char *argv[]) {
CLIENT *cl;
int answer;
record *rec = (record *) malloc(sizeof(record));
if (argc != 4) {
printf("Usage: %s server arg1 arg2\n", argv[0]);
exit (1);
}
if (!(cl = clnt_create(argv[1], ADDITPROG,ADDITVERS,"tcp"))) {
clnt_pcreateerror(argv[1]);
exit(1);
}

rec->first_num = atoi(argv[2]);
rec->second_num= atoi(argv[3]);
answer = *add_args_1(rec,cl);
if (answer <= 0) {
printf("error: could not produce meaningful results");
exit(1);
}
printf("%s + %s = %d\n", argv[2], argv[3], answer);
}

