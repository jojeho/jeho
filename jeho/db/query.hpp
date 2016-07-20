#pragma once
#include "boost/type_index.hpp"
#include <list>
#include "parser.hpp"
namespace jeho { namespace db {

    template<typename T>
    std::string type_name()
    {
      return boost::typeindex::type_id<T>().pretty_name();
    }

    struct gt
    {};

    struct lt
    {};

    struct ge
    {};

    struct le
    {};

    struct eq
    {};

    struct and_
    {};

    struct or_
    {};
     
    struct query
    {
      std::string str;
      query(std::string q):str(q)
      {}
      
    };
    
  }}
