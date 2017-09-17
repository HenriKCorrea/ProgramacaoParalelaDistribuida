/* this code will be translated into the needed stubs and headers */
/* use: rpcgen addit.x; */
struct record {
int first_num;
int second_num;
};
/* arguments for RPC must be one single */
/* value or a structure of values */
/* first_num and second_num are addends */
program ADDITPROG {             /*value to register the program */
version ADDITVERS {             /*version must be assigned a value */
int ADD_ARGS(record) = 1;       /*this is the service function */
} =1;                           /*version value */
} = 0x20000003;                 /*program value */
