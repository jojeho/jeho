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
    req_res(mongo::connection const& con , jeho::db::query q):cursor<T>(con, q){}
  };

  struct non{};

  template<typename T>
  struct req_up<insert,T,jeho::db::query> 
  {
    mongo::connection const&c ;
    req_up(mongo::connection const& con , jeho::db::query const&q):c(con){}

    insert_iterator<T> begin()
    {
      return insert_iterator<T>(c);
    }
  };
  
}
