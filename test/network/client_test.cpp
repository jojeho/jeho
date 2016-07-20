//#include "jeho/network/req_res.hpp"
//#include  "server_config.hpp"
#include <chrono>
#include <string>
#include <boost/hana.hpp>
#include <jeho/network/connection.hpp>
#include <list>


#include "config.hpp"
#include <iostream>

//boost::asio::io_service io_service;

using code = std::string;

int main()
{
  jeho::network::connection con(address , port);
  std::string code ;
  int i;
  auto b =jeho::network::req_res<one_day_stock_base>(con,code,i);
  auto begin = std::begin(b);
  auto end = std::end(b);
  while(begin != end)
    {
      std::cout<<"low price "<<begin->low_price<<std::endl;
      begin++;
    }
}

