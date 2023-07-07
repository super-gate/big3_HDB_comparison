

#ifndef HDB_supergate
#define HDB_supergate


//#include "HDB_supergate_user.hpp"
//#include "HDB_supergate_server.hpp"

#include <helib/helib.h>
using namespace helib;
using namespace NTL;

namespace HDB_supergate_{
    void TestHello();
    
    typedef std::vector<Ctxt> Ctxt_vec;
	typedef std::vector<std::vector<Ctxt>> Ctxt_mat;
    
    /* Query Type */
    enum Q_TYPE_t {
        EQ,
        LT,
		EL,
        MIN,
        MAX
    };

    /* BGV Context Param*/
    struct BGV_param {
        unsigned long p;
        unsigned long d;
        unsigned long m;
        unsigned long nb_primes;
        unsigned long expansion_len;
        unsigned long c;
        unsigned long scale;
        unsigned long r;
    };
 
/*
	p: 167, d: 3, m: 28057, nb_primes: 538, c: 3, l: 4, scale: 6, r: 1
   */

    const struct BGV_param STD128_HDB{
        167,     // p
        3,     // d
        28057,    // m
        538,     // nb_primes
        3,      // c
        4,      // l
        6,      // scale
        1,      // r
    };
    
    const struct BGV_param TOY_HDB{
        7,     // p
        3,     // d
        300,    // m
        200,     // nb_primes
        3,      // c
        3,      // l
        6,      // scale
        1,      // r
    };
    
    Context MakeBGVContext(int p, int d, int m, int nb_primes, int c, int expansion_len, int scale, int r);
 
    Context MakeBGVContext(const struct BGV_param);

};
#endif


