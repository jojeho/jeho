#pragma once
#include "../connection.hpp"
#include "dump_protocol.hpp"

namespace jeho { namespace network { namespace dump{

      struct client
      {
	dump_protocol read_msg_;
	tcp::socket socket_;
        std::shared_ptr<connection> & con;
        client(std::shared_ptr<connection> & c) :con(c), socket_(c.is)
	{
          boost::asio::ip::tcp::socket socket(con->is);
          boost::asio::ip::tcp::resolver resolver(con->is);
          boost::asio::ip::tcp::resolver::query query(con->address, con->port);
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
