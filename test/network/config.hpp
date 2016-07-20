#pragma once
#include "jeho/network/req_res.hpp"
#include <set>
using ptime = std::chrono::system_clock::time_point;
struct my_ss {

  BOOST_HANA_DEFINE_STRUCT(my_ss,
			   (std::string , code),
			   (long ,start_price)
			   );
};


struct stock_base {
  BOOST_HANA_DEFINE_STRUCT(stock_base,
			   (std::string , code),
			   (long ,start_price),
			   (long, high_price),
			   (long, low_price),
			   (long, end_price),
			   (long, trading_volume),
			   (long, trading_money),
			   (ptime, time)
			   );
};


struct one_day_stock_base
{
  using result = std::list<stock_base>;
  using args   = boost::hana::tuple<std::string,int>;

  std::list<stock_base> operator()(std::string const&s , int i)
  {
    std::list<stock_base> result;
    stock_base sb;
    sb.low_price = 111;
    //sb.aa[1] = 33;
    result.push_back(sb);
    sb.low_price = 112;
    //    sb.aa[1] = 44;
    result.push_back(sb);
    std::cout<<"running "<<std::endl;
    return result;
  }
};
struct one_day_stock_base2
{
  using result = stock_base;
  using args   = boost::hana::tuple<std::string,int>;
  stock_base operator()(std::string const&s , int i)
  {
    stock_base sb;
    sb.low_price = 111;
    std::cout<<"running 2"<<std::endl;
    return sb;
  }
};



using funcs = boost::hana::tuple<one_day_stock_base,one_day_stock_base2> ;

using func_request = jeho::network::func_request<funcs>;
using session = jeho::network::req_session<func_request> ;
using server  = jeho::network::server<session> ;
boost::asio::io_service io_service;



const std::string address = "localhost";
const std::string port = "1111";




