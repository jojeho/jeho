#include "jeho/io/binary_archive.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <boost/hana.hpp>
#include <jeho/io/to_string.hpp>

using namespace jeho::io;
struct t
{
  BOOST_HANA_DEFINE_STRUCT(t ,
			   (std::string, name),
			   (int ,i ),
			   (std::chrono::system_clock::time_point,tp)
			   );
  
};

struct m
{
  BOOST_HANA_DEFINE_STRUCT(m ,
			   (t, t1),
			   (std::string,x),
			   (double ,c));
};

int main()
{
  std::stringstream ss;  t t1;
  t1.name ="jeho";
  t1.i = 1;
  t1.tp = std::chrono::system_clock::now();
  m m1;
  m1.t1 = t1;
  m1.x = "xxx";
  m1.c = 0.2;

  
  out<binary_archive> out(ss,m1);

  m m2 ;
  in<binary_archive>  in(ss,m2);

  std::cout<<m2.t1.name<<" "
	   <<m2.t1.i<<" "
	   <<to_string(m2.t1.tp)
	   <<std::endl;
  
  return 0;
}

  
