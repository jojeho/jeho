#include <iostream>
#include <boost/assign.hpp>
#include "jeho/io/csv.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <boost/hana.hpp>
#include <jeho/io/to_string.hpp>

using namespace jeho::io;
using namespace boost::assign;
struct t
{
  BOOST_HANA_DEFINE_STRUCT(t ,
			   (std::string, name),
			   (int ,i ),
			   (double ,c)
			   );
  
};

struct m
{
  BOOST_HANA_DEFINE_STRUCT(m ,
			   (t, t1),
			   (std::string,x),
			   (double ,c));
};

void in_test()
{
  std::list<m> is;
  bool header= false;
  
  in<csv> i("out.txt", is,header);

  for(auto t : is)
    {
      std::cout<<"now "<<t.t1.name<<" "<<t.t1.i<<" "<<t.t1.c
	       <<" "<<t.x<<" "<<t.c
	       <<std::endl;
    }
}


void out_test()
{
  std::list<m> ms;

  t t1 ;
  t1 = t{ "jojeho" , 22 , 32.2};
  ms += m{ t1 , "ccc" , 12.1};

  t1 = t{ "joje2" , 12 , 12.2};
  
  ms += m{ t1 , "xxx" , 132.1};
  
  out<csv> out("out.txt",ms);
}

int main()
{
  out_test();
  in_test();
  
}
