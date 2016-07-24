#pragma once
#include <string>
#include <boost/lexical_cast.hpp>
namespace jeho { namespace network{

    struct connection
    {
      std::string address;
      std::string  port;
      boost::asio::io_service is;
      connection(std::string const&addr
		 , std::string const& p):address(addr),port(p){}


    connection(std::string const&addr
		 , int const& p):address(addr)
				,port(boost::lexical_cast<std::string>(p))
      {}
    };
    
  }}
