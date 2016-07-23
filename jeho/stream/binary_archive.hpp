#pragma once
#include "in.hpp"
#include "out.hpp"

//#include <boost/archive/binary_oarchive.hpp>
//#include <boost/archive/binary_iarchive.hpp>

///**********************************
///binary_archive is fail in case big data. 
///**********************************

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "archive.hpp"

#include "archive.hpp"

namespace jeho { namespace stream {

    struct binary_archive
    {};
    
    template< >
    struct out<binary_archive>
    {
      template<typename Stream,typename T>
      out(Stream&s , T const&t)
      {
	boost::archive::text_oarchive ar(s);
	jeho::stream::operator<<(ar,t);
	//jeho::stream::operator>><boost::archive::binary_oarchive,T>(ar,t);
	//out_from<boost::archive::binary_oarchive,T>(ar,t);
	// boost::hana::for_each(boost::hana::keys(t), [&](auto  b)
	// 		      {
	// 			auto& member = boost::hana::at_key(t ,b);
	// 			using Member = std::remove_reference_t<decltype(member)>;
	// 			out_from<boost::archive::binary_oarchive,Member>(ar,member);
	// 		      });
      }
    };
    
    template<>
    struct in<binary_archive>
    {
      template<typename Stream , typename T>
      in(Stream & s , T &t)
      {
	boost::archive::text_iarchive ar(s);
	//jeho::stream::operator<<<boost::archive::binary_iarchive,T>(ar ,t);
	jeho::stream::operator>>(ar, t);
	//in_to<boost::archive::binary_iarchive,T>(ar, t);
	// boost::hana::for_each(boost::hana::keys(t), [&](auto b)
	// 		      {
	// 			auto& member = boost::hana::at_key(t ,b);
	// 			using Member = std::remove_reference_t<decltype(member)>;
	// 			in_to<boost::archive::binary_iarchive,Member>(ar, member);
	// 			//in_bind<Member>(ar , member);
	// 		      });
      }
    };
  }}
