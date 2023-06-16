#include<stdio.h>
#include<iostream>


#include "HDB_supergate.hpp"


namespace HDB_supergate_ {


    /* COMMON Public code information */
    /*
    enum Q_TYPE_t {
        EQ,
        LT,
        MIN
    };*/
    
    Context MakeBGVContext(int p, int d, int m, int nb_primes, int c, int expansion_len, int scale, int r) {        return ContextBuilder<BGV>()
                    .m(m)
                    .p(p)
                    .r(r)
                    .bits(nb_primes)
                    .c(c) 
                    .scale(scale)
                    .build();
                        
    }    

    Context MakeBGVContext( struct BGV_param params) {        
        return ContextBuilder<BGV>()
                    .m(params.m)
                    .p(params.p)
                    .r(params.r)
                    .bits(params.nb_primes)
                    .c(params.c) 
                    .scale(params.scale)
                    .build();
    }   

    void TestHello() { 
       std::cout << "Hello" << std::endl; 
    }
};

