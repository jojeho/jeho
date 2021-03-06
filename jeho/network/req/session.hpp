#pragma once
#include <boost/lexical_cast.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include "req_protocol.hpp"

//session define way of sending and receiving with client

namespace jeho { namespace network{ namespace req {
    
    template<typename RequestHandle>
    class session : public std::enable_shared_from_this<req::session<RequestHandle>>
    {
    public:
      explicit session(tcp::socket socket)
	: socket_(std::move(socket)),
	  strand_(socket_.get_io_service())
      {}

      ~session()
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
			     boost::asio::read(socket_ ,boost::asio::buffer(data));

			     //no lexical_cast , it's error;
			     long body_length  = std::atol(&data[0]);
			     data.resize(body_length);
			     boost::asio::read(socket_ ,boost::asio::buffer(data));
			     auto result = RequestHandle()(data);
			     if(result)
			       {
			     	 long result_size = result->size();
			     	 data.resize(req_protocol::header_length);
				 std::string length = std::to_string(result_size);
				 std::fill_n(std::begin(data) ,  req_protocol::header_length, ' ');
				 std::copy(std::begin(length),std::end(length) , std::begin(data));
			     	 std::vector<boost::asio::const_buffer> buffers;
			     	 buffers.push_back(boost::asio::buffer(data));
			     	 buffers.push_back(boost::asio::buffer(*result , result_size));
			     	 boost::asio::async_write(socket_ ,buffers,yield);
			       }
			     else
			       {
				 long result_size = 0;
			     	 data.resize(req_protocol::header_length);
				 std::string length = std::to_string(result_size);
				 std::fill_n(std::begin(data) ,  req_protocol::header_length, ' ');
				 std::copy(std::begin(length),std::end(length) , std::begin(data));
			     	 std::vector<boost::asio::const_buffer> buffers;
			     	 buffers.push_back(boost::asio::buffer(data));
			     	 boost::asio::async_write(socket_,boost::asio::buffer(data),yield);
				 
			       }
			     close();
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

    }}}
