#ifndef HDB_supergate_user
#define HDB_supergate_user

#include <helib/helib.h>
#include <NTL/ZZX.h>
//#include <helib/context.h>
#include <NTL/ZZ_pE.h>
#include <NTL/mat_ZZ_pE.h>
#include "../comp_lib/comparator.h"
#include "../comp_lib/tools.h"
#include "HDB_supergate.hpp"


using namespace HDB_supergate_;
using namespace helib;
using namespace NTL;
using namespace std;
using namespace he_cmp;

namespace HDB_supergate_user_{


    void TestHello_user();

    class USER {
        public:
			Comparator & comparator;
            //const helib::Context & _contx;
            //const helib::PubKey & _pk;
            /* It could be removed */
			SecKey & sk;          
            int print;
			int _n; /*lattice dim*/
            int _q_log; /*lattice q*/
            uint32_t lambda; /*Security parameter*/
            
			const EncryptedArray& ea = comparator.m_context.getEA();
            long nslots = ea.size();
            unsigned long p = comparator.m_context.getP();
            unsigned long ord_p = comparator.m_context.getOrdP();
            unsigned long numbers_size = nslots / comparator.m_expansionLen;
			unsigned long enc_base = (p+1)>>1; // UNI
			//unsigned long enc_base = p; //BI
			unsigned long digit_base = power_long(enc_base, comparator.m_slotDeg);
			double result;

            explicit USER(
                //const helib::Context & contx,
                //const helib::PubKey & public_key,
                //const helib::SecKey & secret_key
				Comparator &comparator, 
				SecKey &sk
            );

            ~USER();          
            
			unsigned long max();

			Ctxt Query(int64_t q_id, Q_TYPE_t type, int printer);
                        
            void ShowRes(std::vector<ZZX> datas, Ctxt_vec &less_vector, Ctxt_vec &equal_vector, Ctxt_mat &equal_result, Ctxt_mat &less_result, unsigned long Row, unsigned long num_db_category, unsigned long num_db_element, Q_TYPE_t type);
			
	//		double time_re();

			void debug(Ctxt& ctxt, Comparator& comparator, SecKey& sk);
		
			//unsigned long numbersize();
       private:
            //const helib::SecKey & _sk;

    };
};

#endif


