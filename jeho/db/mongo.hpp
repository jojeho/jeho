#pragma once
#include "../event_pattern.hpp"
#include "query.hpp"
#include "backend/mongo/mongo.hpp"

namespace mongo {


  template<typename Func, typename T,typename Constrain>
  struct req
  {};

  template<typename Func, typename Constrain>
  struct dump
  {};

  template<typename Func, typename T,typename Constrain>
  struct circle
  {};

  
  //using namespace jeho;

  struct select{};
  struct insert{};


  template<typename T>
  struct req<select,T,jeho::db::query> : public mongo::cursor<T>
  {
    req(std::shared_ptr<mongo::connection> const& con , jeho::db::query q):cursor<T>(con, q){}
  };


  template<typename T>
  struct circle<insert,T,jeho::db::query> 
  {
    std::string sub_db;
    std::shared_ptr<mongo::connection> const&c ;
    circle(std::shared_ptr<mongo::connection> const& con , jeho::db::query const&q):c(con),sub_db("")
    {}

    circle(std::shared_ptr<mongo::connection> const& con , std::string const&sdb
	   ,jeho::db::query const&q):c(con),sub_db(sdb)
    {}

    insert_iterator<T> begin()
    {
      if(sub_db != "")
	{
	  return insert_iterator<T>(c , sub_db);
	}
      return insert_iterator<T>(c);
    }
  };
  
}
