#include <jeho/network/execute.hpp>
#include <string>
#include <iostream>
#include <boost/lexical_cast.hpp>
struct test1
{
  int operator()(std::string const&k)
  {
    return 1;
  }
};

struct test2
{
  int operator()(std::string const&k,int const& p)
  {
    return boost::lexical_cast<int>(k) + p ;
  }
};



int main(int argc , char* argv[])
{
  std::string ii ="12";
  boost::hana::tuple<std::string> sc(ii);
  auto b = jeho::network::execute<test1,1>()(sc);
  std::cout<<b<<std::endl;
  int cx=3;

  boost::hana::tuple<std::string ,int> s(ii,cx);
  
  auto c = jeho::network::execute<test2,2>()(s);
  std::cout<<c<<std::endl;
  return 1;
}
