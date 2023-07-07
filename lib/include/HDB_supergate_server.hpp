#ifndef HDB_supergate_server
#define HDB_supergate_server


#include <helib/helib.h>
#include "HDB_supergate.hpp"
#include "../comp_lib/comparator.h"
#include "../comp_lib/tools.h"

using namespace HDB_supergate_;
using namespace helib;
using namespace NTL;
using namespace std;
using namespace he_cmp;

namespace HDB_supergate_server_{
    void TestHello_server();

    class SERVER {
        public:
            //const helib::Context & _contx;
            //const helib::PubKey & _pk;
			Comparator & comparator;
			//SecKey &sk;

            std::vector<std::vector<int64_t>> _db_plaintext;//database type
            Ctxt_mat data_cipher_arr;//<std::shared_ptr<Ctxt>>> data_cipher_arr;//ciphertext data
			

			Ctxt_vec ctxt_equal;
			Ctxt_vec less_final;

			int print;
			unsigned long _num_db_element;
            unsigned long _num_db_category;
            unsigned long _max_element;
			unsigned long _Row;
       
			
			const EncryptedArray& ea = comparator.m_context.getEA();
			long nslots = ea.size();
			unsigned long p = comparator.m_context.getP();
			unsigned long ord_p = comparator.m_context.getOrdP();
			unsigned long numbers_size = nslots / comparator.m_expansionLen;
			unsigned long enc_base = (p+1) >>1; //UNI
			//unsigned long enc_base = p; // BI
			unsigned long digit_base = power_long(enc_base, comparator.m_slotDeg);

            explicit SERVER (
                //const helib::Context & _contx,
                //const helib::PubKey & public_key
				Comparator &comparator
			//	SecKey &sk
                );
           
            ~SERVER ();
            
            void SetDB(
                const std::string & db_dir,
                uint64_t & num_db_element,
                uint64_t & num_db_category,
                uint64_t & max_element);

			std::vector<std::vector<int64_t>> db_return();
           
            void ShowDB() {
                std::cout << "================= DB is ==================" << std::endl; 
                for (int row = 0; row  < _num_db_element; row ++ ) {
                    for (int col = 0 ; col < _num_db_category; col++) {
                        std::cout << _db_plaintext[row][col] << ",";
                    }
                    std::cout << std::endl;
                   
                }
                std::cout << "==========================================" << std::endl; 
               
            }

			//void debug(Ctxt& ctxt, Comparator& comparator, SecKey& sk);
             //std::vector<std::vector<std::shared_ptr<Ctxt>>>
			void Response(Ctxt &query, Q_TYPE_t, int printer); //const;
			
			Ctxt_vec less_vector();

			Ctxt_vec equal_vector();

			Ctxt_mat result_equal();

			Ctxt_mat result_less();

			unsigned long row();

			unsigned long category();

			unsigned long element();

        private:
            /* It could be removed */
          

    };
};

#endif


