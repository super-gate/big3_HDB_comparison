#include<stdio.h>
#include<iostream>
//#include <helib/helib.h>


#include <helib/debugging.h>
#include "HDB_supergate.hpp"
#include "HDB_supergate_server.hpp"
//#include "/home/dohyuk/comparison-circuit-over-fq/code/comparator.h"
#include "../comp_lib/comparator.h"
#include "../comp_lib/tools.h"

using namespace HDB_supergate_;
using namespace helib;
using namespace std;
using namespace he_cmp;
using namespace NTL;

namespace HDB_supergate_server_{
    void TestHello_server() {
        std::cout << "Hello server!" << std::endl;
    }


    /******************************/
    
    /* Construction */
    SERVER::SERVER (
        //const helib::Context & contx,
        //const helib::PubKey & public_key
		Comparator & comparator
//		SecKey & sk
        ) :comparator(comparator)//_contx(contx), _pk(public_key) 
	{


    };
    
    /* Deconstruction */
    SERVER::~SERVER () {

    };
 
	/* Get Response*/
/*
	void SERVER::debug(Ctxt& ctxt, Comparator& comparator, SecKey& sk)
    {
        const EncryptedArray& ea = comparator.m_context.getEA();
        unsigned long ord_p = comparator.m_context.getOrdP();
        long nslots = ea.size();
        vector<ZZX> decrypted_cipher(nslots);
        ea.decrypt(ctxt, sk, decrypted_cipher);

        for(int i=0; i<nslots; i++)
        {
            printZZX(cout, decrypted_cipher[i], ord_p);
            if(decrypted_cipher[i] == ZZX(0))
            {
                cout<<" ";
            }
        }
        cout<<endl;
    };
*/

	//std::vector<std::vector<std::shared_ptr<Ctxt>>> 
	void SERVER::Response(Ctxt &query, Q_TYPE_t types, int printer){
        
		print = printer;
		// Number should be handled
		int q_num = 1;
		 
		unsigned long cipher_num = _Row/numbers_size;

		Ctxt_mat less_mod_ctxt_arr;//less than cipher
	    less_mod_ctxt_arr.resize(cipher_num);

		Ctxt_mat eq_mod_ctxt_arr;//equal cipher
        eq_mod_ctxt_arr.resize(cipher_num);
		
		Ctxt_mat data_mod_ctxt_arr;//data modulo cipher
		Ctxt_vec q_mod_ctxt_arr;//query modulo cipher//std::vector<std::vector<Ctxt>> q_mod_ctxt_arr;

		Ctxt_vec z_ctxt_arr;
		Ctxt_mat z_ctxt_mat;
		Ctxt_vec tmp_vec;

		for(int i = 0; i<cipher_num; i++)
		{
			Ctxt tmp(comparator.m_pk);
			less_final.push_back(tmp);
		}

		if(comparator.m_type == BI)
        {   
            comparator.extract_mod_p(tmp_vec, query);
            q_mod_ctxt_arr = tmp_vec;
			//debug(tmp_vec, comparator, sk);
        }

//UNI

		
		for(int i = 0; i< cipher_num; i++)
		{	
			
			if(comparator.m_type == UNI)
			{
				z_ctxt_arr.push_back(data_cipher_arr[0][i]);
				z_ctxt_arr[i] -= query;
				comparator.extract_mod_p(tmp_vec, z_ctxt_arr[i]);
				z_ctxt_mat.push_back(tmp_vec);
				
				for(long iCoef = 0; iCoef<comparator.m_slotDeg; iCoef++)
				{
					Ctxt ctxt_tmp = Ctxt(comparator.m_pk);
					Ctxt ctxt_tmp_eq = Ctxt(comparator.m_pk);

					comparator.evaluate_univar_less_poly(ctxt_tmp, ctxt_tmp_eq, z_ctxt_mat[i][iCoef]);

					less_mod_ctxt_arr[i].push_back(ctxt_tmp);
					ctxt_tmp_eq.negate();
					ctxt_tmp_eq.addConstant(ZZ(1));
					eq_mod_ctxt_arr[i].push_back(ctxt_tmp_eq);
				}
			}

//BI
    		if(comparator.m_type == BI)
			{
				comparator.extract_mod_p(tmp_vec, data_cipher_arr[0][i]);
				data_mod_ctxt_arr.push_back(tmp_vec);
		
				for(long iCoef = 0; iCoef < comparator.m_slotDeg; iCoef++)// 1 block for whole for// 1 block: m_slotDeg
				{	
					Ctxt ctxt_tmp = Ctxt((data_cipher_arr[0][i]).getPubKey());
					comparator.less_than_bivar(ctxt_tmp, data_mod_ctxt_arr[i][iCoef], q_mod_ctxt_arr[iCoef]);//q_mod_ctxt_arr[i][iCoef]
					less_mod_ctxt_arr[i].push_back(ctxt_tmp);//1 mod in a less block
				}
			
				for(long iCoef = 0; iCoef<comparator.m_slotDeg; iCoef++)
				{
					Ctxt ctxt_z = data_mod_ctxt_arr[i][iCoef];
					ctxt_z -= q_mod_ctxt_arr[iCoef];//q_mod_ctxt_arr[i][iCoef]
					Ctxt ctxt_tmp = Ctxt(ctxt_z.getPubKey());
					comparator.is_zero(ctxt_tmp, ctxt_z);
					eq_mod_ctxt_arr[i].push_back(ctxt_tmp);// eq mod 1 block
				}
			}
			
			Ctxt ctxt_less = less_mod_ctxt_arr[i][comparator.m_slotDeg-1];
			Ctxt ctxt_eq = eq_mod_ctxt_arr[i][comparator.m_slotDeg-1];

			for(long iCoef = comparator.m_slotDeg - 2; iCoef >= 0; iCoef--)
			{
				Ctxt tmp = ctxt_eq;
				tmp.multiplyBy(less_mod_ctxt_arr[i][iCoef]);
				ctxt_less += tmp;

				ctxt_eq.multiplyBy(eq_mod_ctxt_arr[i][iCoef]);
			}

			ctxt_equal.push_back(ctxt_eq);//non modulo equal final

			if(comparator.m_expansionLen == 1)
			{
				less_final.push_back(ctxt_less);//non modulo less ctxt
			}
			else
			{
				comparator.shift_and_mul(ctxt_eq, 0);
				comparator.batch_shift_for_mul(ctxt_eq, 0, -1);

				ctxt_eq.multiplyBy(ctxt_less);
				less_final[i] = ctxt_eq;
				comparator.shift_and_add(less_final[i], 0);
			}
		}
		return;	
    }

	Ctxt_vec SERVER::less_vector() 
	{	
		return less_final;
	}

	Ctxt_vec SERVER::equal_vector()
	{
		return ctxt_equal;
	}
	
	Ctxt_mat SERVER::result_equal()
	{
        unsigned long cipher_num = _Row/numbers_size;

		std::vector<std::vector<Ctxt>> equal_result;
		equal_result.resize(_num_db_category);

		for(int i = 0; i< _num_db_category; i++)
		{	
			for(int j = 0; j< cipher_num; j++)
			{
				Ctxt tmp(comparator.m_pk);
				tmp = data_cipher_arr[i][j];
				tmp.multiplyBy(ctxt_equal[j]);
				equal_result[i].push_back(tmp);
			}
		}

		return equal_result;
	}

	Ctxt_mat SERVER::result_less() 
	{
		std::vector<std::vector<Ctxt>> less_result;

		unsigned long cipher_num = _Row/numbers_size;

		less_result.resize(_num_db_category);

        for(int i = 0; i< _num_db_category; i++)
        {
            for(int j = 0; j< cipher_num; j++)
            {
                Ctxt tmp(comparator.m_pk);
                tmp = data_cipher_arr[i][j];
                tmp.multiplyBy(less_final[j]);
                less_result[i].push_back(tmp);
            }
        }
		return less_result;
	}

	unsigned long SERVER::row()
	{
		return _Row;
	}

	unsigned long SERVER::category()
	{
		return _num_db_category;
	}

	unsigned long SERVER::element()
	{		
		return _num_db_element;
	}

	std::vector<std::vector<int64_t>> SERVER::db_return()
	{
		return _db_plaintext;
	}
    /* Make DB */
    void SERVER::SetDB(
        const std::string &db_dir,
        unsigned long &num_db_element,
        unsigned long &num_db_category,
        unsigned long &max_element
    ) 
	{
	    std::ifstream ifs("../db/"+db_dir);
        if (!ifs.is_open()) {
            std::cerr << "Coudn't open the file: " << db_dir  <<std::endl;
        }

        // Get dim
        //uint32_t dim;
        ifs >> num_db_element;
        ifs >> num_db_category;
        //ifs >> max_element;
        //std::cout << "nde:" << num_db_element << ", ndc: " << num_db_category << ", me: " << max_element << std::endl;
        /* SAVE*/
        _num_db_element = num_db_element;
        _num_db_category = num_db_category;
        _max_element = max_element;
        

        // Lattice
		// if ->num_db element up
        _db_plaintext.resize(num_db_element);
        for (uint32_t row = 0; row < num_db_element; row++) {
            _db_plaintext[row].resize(num_db_category);// memmory allocation
        }
		
		//cout<<"server.setdb"<<endl;
		
        char tmp;
        ifs.get(tmp); // get rid of \n
        for (uint32_t row = 0; row < num_db_element; row++) {
            if (row == 0) {
                ifs.get(tmp); // get rid of "["
                //std::cout << "first get tmp is " << tmp << std::endl;
                //printf("%c \n", tmp);
                ifs.get(tmp); // one more time
                //std::cout << "second get tmp is " << tmp << std::endl;
          
                //printf("%c \n", tmp);
            } else {
                ifs.get(tmp); // get rid of "["
                //std::cout << "first get tmp is " << tmp << std::endl;
          
            }
            for (uint32_t col = 0; col < num_db_category; col++) {
                ifs >> _db_plaintext[row][col];
                //if (col == num_db_category - 1) {
                //    _db_plaintext[row][col].pop_back();
                //}

                //cout << "basis["<<row<<"]["<<col<<"] value is " << basis[row][col] << endl;

            }
	
		 // get rid of "]"
            ifs.get(tmp);
            //std::cout << "last get tmp is " << tmp << std::endl;
            ifs.get(tmp); // get rid of \n
            //std::cout << "get rid of , is " << tmp << std::endl;
            ifs.get(tmp); // get rid of \n
            //std::cout << "get rid of space, is " << tmp << std::endl;
        }

			//db plaintext encrypt
		std::vector<int64_t> zeros;

		unsigned long Row;
		Row = num_db_element;
		
		if(num_db_element % numbers_size != 0)
		{
			int rem = num_db_element % numbers_size;
			rem = numbers_size - rem;
			Row = num_db_element + rem;
			_db_plaintext.resize(Row);//resize plaintext
			//zeros.resize(Row);

			for( int i = 0; i< Row ; i++)
			{
				zeros.push_back(0);//zeros[i] = 0;
			}

			for( int i = 0; i < rem ; i++)
			{
				_db_plaintext[num_db_element + i] = zeros;
			}

		}
		_Row = Row;



		data_cipher_arr.resize(num_db_category);//data cipher matrix resize
	
		for(int i = 0; i< num_db_category ; i++)
		{
			for(int j = 0 ; j< (Row / numbers_size); j++)
			{
				Ctxt tmp(comparator.m_pk);
				data_cipher_arr[i].push_back(tmp);
			}

		}

		int space_bit_size = static_cast<int>(ceil(comparator.m_expansionLen*log2(digit_base)));

		unsigned long input_range = ULONG_MAX;
		if(space_bit_size < 64)
		{
			input_range = power_long(digit_base, comparator.m_expansionLen);
		}

		vector<ZZX> pol_data(nslots);//decomposed element array polynomial(all elements in here)
		ZZX pol_slot;
		unsigned long input_data;

	//	cout<<"input_range: "<<input_range<<endl;

		//cout<<"database encrypted: "<<endl;
		for(int i = 0; i<num_db_category; i++)//columns
		{
			for(int j =0; j<Row ; j = j + numbers_size)//jumping to each cipher block
			{	
				for(int k = 0; k<numbers_size; k++)//ciphertext block
				{	
					input_data = _db_plaintext[j+k][i]%input_range;
					vector<long> decomp_int_data; //decomposition vector of each element
					digit_decomp(decomp_int_data, input_data, digit_base, comparator.m_expansionLen);

					for(int l = 0; l<comparator.m_expansionLen ; l++)
					{
						comparator.int_to_slot(pol_slot, decomp_int_data[l], enc_base);
						pol_data[k*comparator.m_expansionLen+l] = pol_slot; //pol_data: decomposition polynomial
					}
				}
				
				//encrypt from here // j: row of all element, i: all category
				Ctxt ctxt(comparator.m_pk);
				ea.encrypt(ctxt, comparator.m_pk, pol_data);
				data_cipher_arr[i][j/numbers_size] = ctxt;
				//cout<<"database encrypted"<<endl;
				//cout<<ctxt<<" ";
			}
		//	cout<<endl;
		}
/*
		cout<<"database"<<endl;
		for(int i=0; i<num_db_category; i++)
		{
			for(int j=0; j<Row; j= j+numbers_size)
			{
				debug(data_cipher_arr[i][j/numbers_size], comparator, sk);
			}
		}
*/
		if(print == 1)
		{
			cout<<"database encrypted"<<endl;
			for(int i = 0; i<num_db_category; i++)
			{
				for(int j=0; j<Row; j= j+ numbers_size)
				{
					cout<<data_cipher_arr[i][j/numbers_size]<<" ";
				}
				cout<<endl;
			}
		}

        //    // get rid of "]"
        //    ifs.get(tmp);
        //    //std::cout << "last get tmp is " << tmp << std::endl;
        //    ifs.get(tmp); // get rid of \n
        //    //std::cout << "get rid of , is " << tmp << std::endl;
        //    ifs.get(tmp); // get rid of \n
        //    //std::cout << "get rid of space, is " << tmp << std::endl;

			
//        }
        return;

    };


} 

