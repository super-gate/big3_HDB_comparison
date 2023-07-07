#include <stdio.h>
#include <iostream>
#include <helib/debugging.h>


//#include <math>

#include <helib/helib.h>

#include "../../comp_lib/comparator.h"
//#include "../../src/comparison-circuit-over-fq/code/comparator.h"
#include "HDB_supergate.hpp"
#include "HDB_supergate_server.hpp"
#include "HDB_supergate_user.hpp"
#include <NTL/ZZX.h>
#include <time.h>

// NAMESAPCE name should be defferent from its file name...
using namespace HDB_supergate_;
using namespace HDB_supergate_user_;
using namespace HDB_supergate_server_;


/*HELIB*/
using namespace helib;
using namespace std;
using namespace he_cmp;
using namespace NTL;

int main() {
    
    /*Test HELIB CODE */
    /*
    unsigned long p = 7;
    unsigned long d = 10;
    unsigned long m = 300;
    unsigned long nb_primes = 30;
    unsigned long c = 3;
    unsigned long scale = 6;
    unsigned long r = 1;
    */
    

    /*  
    const Context contx = ContextBuilder<BGV>()
                    .m(STD128_HDB.m)
                    .p(STD128_HDB.p)
                    .r(STD128_HDB.r)
                    .bits(STD128_HDB.nb_primes)
                    .c(STD128_HDB.c)
                    .scale(STD128_HDB.scale)
                    .build();
    */
    // const Context contx = MakeBGVContext(STD128_HDB);
    const Context contx = MakeBGVContext(TOY_HDB);
    cout << "Q size: " << contx.logOfProduct(contx.getCtxtPrimes())/log(2.0) << endl;
    cout << "Q*P size: " << contx.logOfProduct(contx.fullPrimes())/log(2.0) << endl;
    cout << "Security: " << contx.securityLevel() << endl;

    /******* END TEST / We start our proposal codes ***********/
    
    cout<<"///////////////////////////////////"<<endl;
	contx.getZMStar().printout();
	cout<<endl;
	cout<<"/////////////////////////////////////"<<endl;


    /* 나중엔 User의 API로만 세팅할 수 있음*/
    /* 일단 지금은 secret key가 sharing 되어있다는 가정*/

    /*User side, Gene Keys*/
    SecKey secret_key(contx); 
    secret_key.GenSecKey();
	
    if (TOY_HDB.expansion_len > 1)
	{
		if (contx.getZMStar().numOfGens() == 1)
		{
			std::set<long> automVals;
			long e = 1;
			long ord = contx.getZMStar().OrderOf(0);
			bool native = contx.getZMStar().SameOrd(0);
			if(!native)
				automVals.insert(contx.getZMStar().genToPow(0, -ord));
			while (e < TOY_HDB.expansion_len){
				long atm = contx.getZMStar().genToPow(0, ord-e);
//				cout << "Automorphism " << -e << " is " << atm << endl;
				automVals.insert(atm);
				e <<=1;
			}
			addTheseMatrices(secret_key, automVals);
		}
		else
		{
		addSome1DMatrices(secret_key);
		}
	}

	if (TOY_HDB.d>1)
		addFrbMatrices(secret_key);
	// make public key
	PubKey public_key(secret_key);
	
	CircuitType type = BI; //UNI or BI
	
	Comparator comparator(contx, type, TOY_HDB.d, TOY_HDB.expansion_len, public_key, false); // secret key deleted. only public key remained
	const EncryptedArray& ea = comparator.m_context.getEA();// erase!!
	long nslots = ea.size();

	/*Secret key is contained in this class. Be carefull! */
    USER user = USER(comparator, secret_key); //pass secret key only to user
	/*SERVER SIDE, Preparing DBs */   
    SERVER server = SERVER(comparator);
	std::string db_dir = "db2.txt";
    long unsigned num_db_element;
    long unsigned num_db_category;
    long unsigned max_element;
	std::vector<std::vector<int64_t>> database;

    server.SetDB(db_dir, num_db_element, max_element, num_db_category);
	
	//database = server.db_return();

	server.ShowDB();

    /*USER SIDE, Make the query and send it to te server */

    /*0: exact, 1: less then, 2: min */
    string str;

    Q_TYPE_t types;

	int print_ctxt;

	while(true)
	{
		cout<<"the type has to be one of (eq, lt, el)."<<endl;
		cout<<"input type: ";
		cin>>str;
		if(str == "eq")
		{
			types = EQ;
			break;
		}
		else if(str == "lt")
		{
			types = LT;
			break;
		}
		else if(str == "el")
		{
			types = EL;
			break;
		}
	}

	unsigned long max = user.max();

	int64_t query_id;
	while(true)
	{
		cout<<"maximum int is "<<max - 1<<endl;
		cout<<"input query: ";
		cin>>query_id;

		if(query_id < max)
		{
			break;
		}
	}

	while(true)
	{
		cout<<"print ciphertext?(y/n) ";
		cin>>str;
		if(str == "y")
		{
			print_ctxt = 1;
			break;
		}	
		else if(str == "n")
		{
			print_ctxt = 0;
			break;
		}
	}

	auto ct_query = user.Query(query_id, types, print_ctxt);

	/*ct_query_exact is sended to the server. */
    /*SERVER SIDE, / Exact / Less than /(or min)/ calculation and return to the user  */
    
	server.Response(ct_query, types, print_ctxt);

	
	Ctxt_vec less_vector;
	Ctxt_vec equal_vector;
	Ctxt_mat equal_result;
	Ctxt_mat less_result;
	unsigned long Row;
	unsigned long Category;
	unsigned long Element;

	less_vector = server.less_vector(); 
	less_result = server.result_less(); 
	equal_vector = server.equal_vector();
	for (auto& ctxt: equal_vector)
	{
		vector<ZZX> ptxt;
		contx.getView().decrypt(ctxt, secret_key, ptxt);
		for (auto& zzx: ptxt)
		{
			printZZX(cout, zzx, contx.getOrdP());
            cout << ", ";
		}
		cout << endl;
	}
	equal_result = server.result_equal();
	Row = server.row();
	Category = server.category();
	Element = server.element();
	
	database = server.db_return();
	
	std::vector<ZZX> expected(Element);
	cout<<"plaintext data"<<endl;
	for(long i = 0; i<Element; i++)
	{
		//cout<<database[i][0]<<" ";
		if(types == LT)
		{
			if(database[i][0] < query_id)
			{
				expected[i] = ZZX(1);
			}
			else
			{
				expected[i] = ZZX(0);
			}
		}
		else if(types == EQ)
		{
			if(database[i][0] == query_id)
			{
				expected[i] =ZZX(1);
			}
			else
			{
				expected[i] = ZZX(0);
			}
		}
		else if(types == EL)
		{
			if(database[i][0] <= query_id)
			{
				expected[i] = ZZX(1);
			}
			else
			{
				expected[i] = ZZX(0);
			}
		}
	}

	cout<<"expected"<<endl;
	std::vector<ZZX> datas;
	for(int i = 0; i< Element; i++)
	{
		cout<<expected[i] * ZZX(database[i][0])<<" ";
		datas.push_back(ZZX(database[i][0]) * expected[i]);
	}
	cout<<endl;
	cout<<endl;

	//int a = int(expected[10]);

    /*answer is sended to the user */
    user.ShowRes(datas, less_vector, equal_vector, equal_result, less_result, Row, Category, Element, types);

    cout << "Test End!! " << endl;

    return 0;
}



