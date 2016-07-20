#pragma once
#include <boost/fusion/adapted/struct.hpp>
namespace jeho { namespace db {

    namespace fusion = boost::fusion;
    struct column_value_map
    {
      std::string column ;
      std::string value ;
      std::string op;
      //std::string big_op;
    };

    struct sentence
    {
      column_value_map cv;
      std::string op;
    };

  }}


BOOST_FUSION_ADAPT_STRUCT(
    jeho::db::column_value_map ,
    (std::string , column)
    (std::string , op)
    (std::string , value))

BOOST_FUSION_ADAPT_STRUCT(
    jeho::db::sentence ,
    (jeho::db::column_value_map ,cv)
    (std::string , op))

  
