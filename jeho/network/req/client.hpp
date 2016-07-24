#pragma once
#include <boost/asio.hpp>
#include "req_protocol.hpp"
#include "../connection.hpp"
namespace jeho {
  namespace network {
    namespace req {
      using  boost::asio::ip::tcp;
      struct client
      {
	req_protocol read_msg_;
	tcp::socket socket_;
	std::unique_ptr<connection> & con;
	 client(std::unique_ptr<connection> & c) :con(c),socket_(con->is)
	{
	  boost::asio::ip::tcp::resolver resolver(con->is);
	  boost::asio::ip::tcp::resolver::query query(con->address, con->port);
	  auto endpoint_iterator = resolver.resolve(query);
	  boost::asio::connect(socket_, endpoint_iterator);
	}

	std::string request(std::string const& args)
	{
	  std::vector<char> data(req_protocol::header_length);
	  std::fill_n(std::begin(data) ,  req_protocol::header_length, ' ');
	  std::string length = std::to_string(args.size());
	  std::copy(std::begin(length), std::end(length), std::begin(data));
	  std::vector<boost::asio::const_buffer> buffers;
	  buffers.push_back(boost::asio::buffer(data));
	  buffers.push_back(boost::asio::buffer(args));
	  boost::asio::write(socket_, buffers);

	  ///read 
	  boost::asio::read(socket_, boost::asio::buffer(data));
	  //no lexical_cast , it's error;
	  long body_length = std::atol(&data[0]);

	  if (body_length == 0)
	    {
	      return std::to_string(body_length);
	    }

	  data.resize(body_length);
	  boost::asio::read(socket_, boost::asio::buffer(data));
	  return std::string(std::begin(data), std::end(data));
	}
      };
    }
  }
}
