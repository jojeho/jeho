#pragma once
#include "jeho/req_pattern.hpp"
#include "mongo.hpp"


namespace mongo {

  using namespace jeho;

  struct select{};
  struct insert{};

  template<typename Func, typename T,typename Constrain>
  struct req_res
  {};

  template<typename Func, typename T,typename Constrain>
  struct req_up
  {};


  template<typename T>
  struct req_res<select,T,jeho::db::query> : public mongo::cursor<T>
  {
    req_res(std::shared_ptr<mongo::connection> const& con , jeho::db::query q):cursor<T>(con, q){}
  };

  struct non{};

  template<typename T>
  struct req_up<insert,T,jeho::db::query> 
  {
    std::string sub_db;
    std::shared_ptr<mongo::connection> const&c ;
    req_up(std::shared_ptr<mongo::connection> const& con , jeho::db::query const&q):c(con),sub_db("")
    {}
    req_up(std::shared_ptr<mongo::connection> const& con , std::string const&sdb
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
