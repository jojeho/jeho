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
#include "req_client.hpp"
namespace jeho { namespace network {
	
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
