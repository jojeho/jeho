
#pragma once
#include "in.hpp"
#include "out.hpp"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "archive.hpp"

namespace jeho { namespace io {

    struct text_archive
    {};
    
    template< >
    struct out<text_archive>
    {
      template<typename Stream,typename T>
      out(Stream&s , T const&t)
      {
	boost::archive::text_oarchive ar(s);
	jeho::io::operator<<(ar,t);
      }
    };
    
    template<>
    struct in<text_archive>
    {
      template<typename Stream , typename T>
      in(Stream & s , T &t)
      {
	boost::archive::text_iarchive ar(s);
	jeho::io::operator>>(ar, t);
      }
    };
  }}
