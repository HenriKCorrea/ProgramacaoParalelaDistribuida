/* add_svc.c server code for the example application */
#include <stdio.h>
#include <string.h>
#include <rpc/rpc.h>
#include "addit.h"

int *add_args_1_svc(record *rec, struct svc_req *req) {
static int result;
result = rec->first_num + rec->second_num;
return ((int *) &result);
}