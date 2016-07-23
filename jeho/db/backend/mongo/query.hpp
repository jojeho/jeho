#pragma once
#include <boost/optional.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/hana.hpp>

#include "../query.hpp"
#include "common.hpp"

#include <iomanip>

namespace mongo {

using  ptime = std::chrono::time_point<std::chrono::system_clock>;  

  std::string to_string(ptime const&t)
  {
    using namespace std::chrono;
    std::time_t tx = system_clock::to_time_t(t);
    return std::string(std::ctime(&tx));
  }
  
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::finalize;

        // filter_builder << "$or" << open_array << open_document << "cuisine"
        //                << "Italian" << close_document << open_document << "address.zipcode"
        //                << "10075" << close_document << close_array;

  std::string convert_op(std::string e)
  {
    if(e == "<")
      {
	return "$lt";
      }
    else if(e == ">")
      {
	return "$gt";
      }
    else if( e =="<=")
      {
	return "$lte";
      }
    else if( e ==">=")
      {
	return "$gte";
      }
    else if(e == "!")
      {
	return "$ne";
      }
    else if(e =="=")
      {
	return "";
      }

    std::string msg("no operator in parsing query");
    msg +=e;
    throw std::runtime_error(msg);

    return "";
  }



  bsoncxx::types::b_date from_string(std::string const&value)
  {
    using namespace std::chrono;
    std::tm t ={};
    std::istringstream ss(value);
    //for koream lanege 
    //ss.imbue(std::locale("de_DE.utf-8"));
    ss>>std::get_time(&t, "%Y%m%d");
    std::time_t tt = std::mktime (&t);
    system_clock::time_point tp = system_clock::from_time_t (tt);

    //debug check
    //std::cout<<std::put_time(&t, "%c")<<std::endl;
    //std::time_t tx = system_clock::to_time_t(tp);
    //  std::cout << std::ctime(&tx) << std::endl; // 

    return bsoncxx::types::b_date{tp};
  }

  template<typename T
	   ,typename std::enable_if<std::is_same<T, ptime>::value,T>::type* = nullptr
	   >
  void convert_value(
  		bsoncxx::builder::stream::document & doc
  		,std::string const& column
  		,std::string const& value
		,std::string const& op
		     )
  {
    auto bt = from_string(value);
    //auto y= std::chrono::system_clock::now();
     if(op != "")
       doc<<column<<open_document<<op<<bt<<close_document;
    else
      doc<<column<<bt;
  }

  template<typename T
	   ,typename std::enable_if<!std::is_same<T, ptime>::value ,T>::type* = nullptr>
  void convert_value(
  		     bsoncxx::builder::stream::document & doc
  		     ,std::string const&column
  		     ,std::string const& value
		     ,std::string const& op

  		     )
  {
    //std::cout<<"xx" <<column<<" "<<value<<" "<<op<<std::endl;

    if(op != "")
      doc<<column<<open_document<<op<<boost::lexical_cast<T>(value)<<close_document;
    else
      doc<<column<<boost::lexical_cast<T>(value);

    //doc<<"start_price"<<open_document<<"$lt"<<
  }

  template<typename T>
  bsoncxx::builder::stream::document
  make_query(jeho::db::query const&q)
  {
    T t;
    bsoncxx::builder::stream::document filter_builder;
    auto st = jeho::db::sentences(q.str);
    for(auto s : st)
      {
    	auto op = convert_op(s.cv.op);
	auto value = s.cv.value;

	boost::hana::for_each(boost::hana::keys(t) , [&](auto key)
	 		      {
				auto name = std::string(boost::hana::to<const char*>(key));
				if(name == s.cv.column)
				  {
				    auto& member = boost::hana::at_key(t, key);
				    using Member = std::remove_reference_t<decltype(member)>;
				    convert_value<Member>(filter_builder , s.cv.column ,  s.cv.value
							  ,op);
				  }
	 		      });
      }

    //    filter_builder<<finalize;
    return filter_builder;
  }

}
