#pragma once
#include "boost/type_index.hpp"
#include <boost/hana.hpp>
#include <list>
#include "parser.hpp"
namespace jeho { namespace db {

    auto has_table_name = boost::hana::is_valid([](auto&& obj) -> decltype(obj.table_name()) { });

    template<typename T>
    std::string table_name()
    {
      T obj;
      return boost::hana::if_(has_table_name(obj),
		       [](auto& x) { return x.table_name(); },
		       [](auto& x) { return  boost::typeindex::type_id<T>().pretty_name();}
		       )(obj);
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
