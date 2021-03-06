#pragma once
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include "connection.hpp"

namespace jeho { namespace network{

using boost::asio::ip::tcp;
using namespace boost::asio;
using namespace boost::asio::ip;

template<typename Session>
struct server
{
  io_service ioservice;

  server(connection const&con)
  {
    try
      {
	boost::asio::io_service io_service;

	boost::asio::spawn(io_service,
			   [&](boost::asio::yield_context yield)
			   {
			     tcp::acceptor acceptor(io_service,
						    tcp::endpoint(tcp::v4()
								  ,boost::lexical_cast<int>(con.port)));

			     for (;;)
			       {
				 boost::system::error_code ec;
				 tcp::socket socket(io_service);
				 acceptor.async_accept(socket, yield[ec]);
				 if (!ec) std::make_shared<Session>(std::move(socket))->go();
			       }
			   });

	io_service.run();
      }
    catch (std::exception& e)
      {
	std::cerr << "Exception: " << e.what() << "\n";
      }
  }

};


    
}}
