#pragma once
#include "dump_protocol.hpp"
namespace jeho { namespace network { namespace dump{

      template<typename Handlers>
      class session : public std::enable_shared_from_this<session<Handlers>>
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
			       std::vector<char> data(dump_protocol::header_length);
			       std::fill_n(std::begin(data) ,  dump_protocol::header_length, ' ');
			       boost::asio::read(socket_ ,boost::asio::buffer(data));

			       //no lexical_cast , it's error;
			       long body_length  = std::atol(&data[0]);
			       //std::cout<<" body lenth "<<body_length<<std::endl;
			       data.resize(body_length);
			       boost::asio::read(socket_ ,boost::asio::buffer(data));
			       close();
			       Handlers()(data);
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
