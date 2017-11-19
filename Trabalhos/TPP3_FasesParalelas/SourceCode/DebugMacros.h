#ifndef DEBUGMACROS__H
#define DEBUGMACROS__H

#include <iostream>

#ifdef DEBUG
    #define DBG_VAR(VAR)\
        std::cout << "DBG: " << __FILE__ << "(" << __LINE__ << ") "\
        << #VAR << " = [" << (VAR) << "]" << std::endl

    #define DBG_MSG(MSG)\
        std::cout << "DBG: " << __FILE__ << "(" << __LINE__ << ") - "\
        << (MSG) << std::endl    
    
    #define DBG_VECTOR(V, S) \
        do{\
            printf("%s: ", #V);\
            for(int i = 0; i < (S); ++i)\
                printf("[%03d] ", V[i]);\
            printf("\n");\
        }while(0)
#else
    #define DBG_VAR(VAR)
    #define DBG_MSG(MSG)
    #define DBG_VECTOR(V, S)
#endif

#endif //DEBUGMACROS__H