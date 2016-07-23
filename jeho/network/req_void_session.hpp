#pragma once
#include <boost/lexical_cast.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include "req_protocol.hpp"

//session define way of sending and receiving with client

namespace jeho { namespace network{
    
    template<typename RequestHandle>
    class req_void_session : public std::enable_shared_from_this<req_session<RequestHandle>>
    {
    public:
      explicit req_session(tcp::socket socket)
	: socket_(std::move(socket)),
	  strand_(socket_.get_io_service())
      {}

      ~req_void_session()
      {
	//std::cout<<"leave request session"<<std::endl;
      }

      void go()
      {
	auto self(this->shared_from_this());
	boost::asio::spawn(strand_,
	[this, self](boost::asio::yield_context yield)
			   {
			     std::vector<char> data(req_protocol::header_length);
			     std::fill_n(std::begin(data) ,  req_protocol::header_length, ' ');
			     boost::asio::read(socket_ ,boost::asio::buffer(data));

			     //no lexical_cast , it's error;
			     long body_length  = std::atol(&data[0]);
			     data.resize(body_length);
			     boost::asio::read(socket_ ,boost::asio::buffer(data));
			     close();
			     RequestHandle()(data);
			   });
      }

      void close()
      {
	socket_.close();
      }

    private:
      tcp::socket socket_;
      boost::asio::io_service::strand strand_;
      };

    }}
