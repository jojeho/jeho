#pragma once
#include <list>

#include "jeho/network/connection.hpp"
#include "jeho/network/server.hpp"
#include "jeho/network/req_session.hpp"
#include "jeho/network/func_request.hpp"

#include <boost/asio.hpp>
#include <iterator>
#include <jeho/stream/binary_archive.hpp>
#include "req_protocol.hpp"

namespace jeho { namespace network {


    using  boost::asio::ip::tcp;
    struct req_res_client
    {
      req_protocol read_msg_;
      tcp::socket socket_;
      boost::asio::io_service & is_;
      req_res_client(connection const& con,
		 boost::asio::io_service& is):is_(is),socket_(is)
      {
	boost::asio::ip::tcp::socket socket(is);
	boost::asio::ip::tcp::resolver resolver(is);
	boost::asio::ip::tcp::resolver::query query(con.address , con.port );
	auto endpoint_iterator = resolver.resolve(query);
	boost::asio::connect(socket_, endpoint_iterator);
      }
      
      std::string request(std::string const& args)
      {
	std::vector<char> data(req_protocol::header_length);
	std::string length = std::to_string(args.size());
	std::copy(std::begin(length),std::end(length) , std::begin(data));
	std::vector<boost::asio::const_buffer> buffers;
	buffers.push_back(boost::asio::buffer(data));
	buffers.push_back(boost::asio::buffer(args ,args.size()));
	boost::asio::write(socket_ , buffers); 

	///read 
	boost::asio::read(socket_ , boost::asio::buffer(data));
	//no lexical_cast , it's error;
	long body_length = std::atol(&data[0]);

	if(body_length == 0)
	  {
	    return std::to_string(body_length);
	  }
	
	data.resize(body_length);
	boost::asio::read(socket_ ,boost::asio::buffer(data));
	return std::string(std::begin(data) , std::end(data));
      }
    };



    template<typename Func>
    struct req_res
    {
      //using iterator = typename std::list<Result>::iterator;
      //bstd::list<typename Func::result> rs;
      using result_t = typename Func::result;
      using args_t    = typename Func::args;
      result_t results;

      template<typename ...Args>
      req_res(jeho::network::connection const&con
	      ,Args... args)
      {
	// stream ss;
	boost::asio::io_service io;

	func_format<Func> f;
	f.args = boost::hana::make_tuple(args...);
	std::string func_name = typeid(Func).name();
	std::stringstream ss;
	ss<<func_name<<":";
	  jeho::stream::out<jeho::stream::binary_archive>(ss ,f);
	jeho::network::req_res_client client(con,io);
	auto result = client.request(ss.str());
	if(result == "0")
	  {
	    std::cout<<"can't find function "<<func_name<<std::endl;
	  }

	std::stringstream is(result);
	jeho::stream::in<jeho::stream::binary_archive>(is ,results);
      }

      auto begin() -> decltype(std::begin(results))
      {
	return std::begin(results);
      }

  
      auto end() ->decltype(std::end(results))
      {
	return std::end(results);
      }

    };
  }}
