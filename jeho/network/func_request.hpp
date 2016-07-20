#pragma once
#include <boost/optional.hpp>
#include <jeho/stream/binary_archive.hpp>

#include "execute.hpp"


template<typename Func>
struct func_format
{
  using Args =  typename Func::args ;
  func_format()
  {}

  BOOST_HANA_DEFINE_STRUCT(func_format ,
			   (Args , args)
			   );
};

namespace jeho { namespace network {

    template<typename Funcs>
    struct func_request
    {
      boost::optional<std::string> operator()(std::vector<char> const& data)
      {
	auto it = std::find(data.begin() , data.end() , ':');
	std::string func_name = std::string(data.begin() , it);
	it++;
	std::string text(it , data.end());

	std::string result;
	
	{
	  Funcs funcs;
	  boost::hana::for_each(funcs , [&](auto func)
				{
				  if(typeid(decltype(func)).name() == func_name)
				    {
				      std::stringstream ss(text);
				      func_format<decltype(func)>  ff;
				      jeho::stream::in<jeho::stream::binary_archive> in(ss , ff);
				      auto r =execute<decltype(func),boost::hana::length(ff.args)>()(ff.args);
				      std::stringstream out_ss;
				      jeho::stream::out<jeho::stream::binary_archive> out(out_ss,r);
				      result = out_ss.str();
				    }
				});

	  if(result == "")
	    {
	      return boost::optional<std::string>();
	    }
	}

	return result;;
	//return serialize(s);
      }
    };

  }}
