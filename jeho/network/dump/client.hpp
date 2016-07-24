#pragma once
#include "../connecton.hpp"
#include "dump_protocol.hpp"

namespace jeho { namespace network { namespace dump{

      struct client
      {
	dump_protocol read_msg_;
	tcp::socket socket_;
	boost::asio::io_service & is_;
        client(connection const& con) :is_(con.is), socket_(is)
	{
	  boost::asio::ip::tcp::socket socket(is);
	  boost::asio::ip::tcp::resolver resolver(is);
	  boost::asio::ip::tcp::resolver::query query(con.address, con.port);
	  auto endpoint_iterator = resolver.resolve(query);
	  boost::asio::connect(socket_, endpoint_iterator);
	}

        void send(std::string const& text)
        {
	  std::vector<char> data(req_protocol::header_length);
          std::fill_n(std::begin(data) ,  dump_protocol::header_length, ' ');
          std::string length = std::to_string(text.size());
	  std::copy(std::begin(length), std::end(length), std::begin(data));
	  std::vector<boost::asio::const_buffer> buffers;
	  buffers.push_back(boost::asio::buffer(data));
	  buffers.push_back(boost::asio::buffer(args));
	  boost::asio::write(socket_, buffers);
	}
      };
 }}}
