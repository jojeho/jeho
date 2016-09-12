#pragma once
#include <iterator>
#include <type_traits>
#include <chrono>

#include "query.hpp"
#include  "common.hpp"
#include <jeho/db/meta.hpp>
#include "boost/hana.hpp"
#include "boost/lexical_cast.hpp"
#include <jeho/io/text_archive.hpp>

using  ptime = std::chrono::time_point<std::chrono::system_clock>;

namespace mongo {

  using namespace jeho::db;
  
  template <typename T>
  std::enable_if_t<std::is_same<T, ptime>::value,
		   ptime> bind(bsoncxx::document::element const& ele) {
    return T(ele.get_date());
  }

  template <typename T>
  std::enable_if_t<boost::hana::Struct<T>::value,
		   T> bind(bsoncxx::document::element const& ele) {
    std::string s(ele.get_utf8().value.to_string().data());

    T t;
    std::stringstream ss(s);
    jeho::io::in<jeho::io::text_archive> in(ss,t);
    return t;
  }

  template <typename T>
  std::enable_if_t<is_vector<T>::value,
		   T> bind(bsoncxx::document::element const& ele) {
    std::string s(ele.get_utf8().value.to_string().data());

    T t;
    std::stringstream ss(s);
    jeho::io::in<jeho::io::text_archive> in(ss,t);
    return t;
  }

  
  
  template <typename T>
  std::enable_if_t<std::is_same<T, long>::value,
		   long> bind(bsoncxx::document::element const& ele) {
    return ele.get_int64();
  }

  template <typename T>
  std::enable_if_t<std::is_same<T, int>::value,
		   long> bind(bsoncxx::document::element const& ele) {
    return ele.get_int32();
  }


  template <typename T>
  std::enable_if_t<std::is_same<T, std::string>::value,
		   std::string> bind(bsoncxx::document::element const& ele) {
    return std::string(ele.get_utf8().value.to_string().data());
  }

  template <typename T>
  std::enable_if_t<std::is_same<T, double>::value,
		   double> bind(bsoncxx::document::element const& ele) {
    return ele.get_double();
  }


  template <typename T>
  std::enable_if_t<std::is_same<T, ptime>::value,
		   bsoncxx::types::b_date> to(T const& t) {

    auto x = std::chrono::duration_cast<std::chrono::milliseconds>(t.time_since_epoch());
    return bsoncxx::types::b_date{x};
  }

      
  template <typename T>
  std::enable_if_t<boost::hana::Struct<T>::value,
		   std::string 
		   >
  to(T const& t)
  {
    std::stringstream ss;
    jeho::io::out<jeho::io::text_archive> out(ss , t);
    return ss.str();
  }


  template <typename T>
  std::enable_if_t<is_vector<T>::value,
		   std::string 
		   >
  to(T const& t)
  {
    std::stringstream ss;
    jeho::io::out<jeho::io::text_archive> out(ss , t);
    return ss.str();
  }

  
  template <typename T>
  std::enable_if_t<!std::is_same<T, ptime>::value
		   &&!boost::hana::Struct<T>::value
		   &&!is_vector<T>::value
		   ,T>
  to(T const& in) {
    return in;
  }


struct connection
{
  mongocxx::client conn; 
  mongocxx::database db;
  std::string db_name_;
  connection(std::string const& constring,std::string const& db_name):db_name_(db_name)
  {
    //single_init::Instance();
    //mongocxx::instance inst{};
    conn =  mongocxx::client{mongocxx::uri{constring}};
    db = conn[db_name];
    //std::cout<<"connected to "<<constring<<" db:"<<db_name<<std::endl;

  }

  const std::string& db_name() const
  {
    return db_name_;
  }
  //connection(){}
};

  template<typename T>
  struct cursor 
  {
    class iterator;
    std::shared_ptr<mongocxx::collection> col;
    std::shared_ptr<mongocxx::cursor> cur;
    bsoncxx::builder::stream::document query;
    cursor(std::shared_ptr<connection> con, jeho::db::query const& q)
    {
      col = std::make_shared<mongocxx::collection>(con->db[jeho::db::table_name<T>()]);
      query = make_query<T>(q);
      cur = std::make_shared<mongocxx::cursor>(col->find(query.view()));
    }

    cursor(std::shared_ptr<connection> const& con
	   ,std::string sub_db
	   ,jeho::db::query const& q)
    {
      col = std::make_shared<mongocxx::collection>(con->db[jeho::db::table_name<T>() + "." + sub_db]);
      //std::cout<<sub_db +"." + jeho::db::type_name<T>()<<std::endl;
      query = make_query<T>(q);
      cur = std::make_shared<mongocxx::cursor>(col->find(query.view()));
    }


    cursor(){}
    
    iterator begin()
    {
      cur = std::make_shared<mongocxx::cursor>(col->find(query.view()));
      //cur = std::make_shared<mongocxx::cursor>(col->find({}));
      return iterator(std::begin(*cur));
    }

    iterator end()
    {
      return iterator(std::end(*cur));
    }
  };
  
  template<typename T>
  struct cursor<T>::iterator : public std::iterator<std::input_iterator_tag , T>
  {

    cursor<T>::iterator& operator++() 
    {
      (it)++;
      return *this;
    }

    void operator++(int) {
      operator++();
    }

    
    T operator*()
    {
      T result;
      auto begin = std::begin(*it);
      auto end = std::end(*it);
      boost::hana::for_each(boost::hana::keys(result), [&](auto key) {
	  auto name = std::string(boost::hana::to<const char*>(key));
	  //std::cout<<name<<std::endl;
	  auto ele = std::find_if(begin , end , [&](auto const& n)
				  {
				    return n.key().to_string() == name;
				  });

	  auto& member = boost::hana::at_key(result, key);
	  using Member = std::remove_reference_t<decltype(member)>;
	  member = bind<Member>(*ele);
	});
      return result;
    }
    
    ~iterator()
    {
      //std::cout<<"delte"<<std::endl;
    }

  private:

    friend class cursor<T> ;

    mongocxx::cursor::iterator it;
    //cursor<T> *c_;
    explicit iterator(mongocxx::cursor::iterator i):it(i)
    {}

    
    friend bool operator ==(const iterator & lhs , const iterator & rhs)
    {
      if(lhs.it == rhs.it) return true;
      
      return false;
    }

    friend bool operator != (iterator const&lhs ,
			     iterator const&rhs)
    {
      return (lhs.it) != (rhs.it);
    }
  };

  template<typename T>
  struct insert_iterator:std::iterator<std::input_iterator_tag , T>
  {
    typedef insert_iterator<T> self;
    mongocxx::collection col_;
    std::shared_ptr<connection>  con_;
    T t_;
    insert_iterator(std::shared_ptr<connection>  con):con_(con)
    {
      auto name = jeho::db::table_name<T>();
      //std::cout<<" name "<<name<<std::endl;
      col_ = con_->db[name];
    }

    insert_iterator(std::shared_ptr<connection>  con ,std::string const& sub_db):con_(con)
    {
      auto table_name = jeho::db::table_name<T>() + "." + sub_db;
      col_ = con_->db[table_name];
    }
    
    self& operator++()
    {
      bsoncxx::builder::stream::document doc{};
      boost::hana::for_each(t_ , [&](auto const& pair)
			    {
			      auto name = boost::hana::to<char const*>(boost::hana::first(pair));
			      std::string c = std::string(name);
			      auto & value  = boost::hana::second(pair);
			      doc<<c<<to(value);
			    });

      col_.insert_one(doc.view());
      return *this;
    }

    void operator++(int)
    {
      operator++();
    }

    T& operator*()
    {
      return t_;
    }
    
  };

  template<typename T>
  void remove_table(std::shared_ptr<connection>  con , std::string sub_db)
  {
    auto col = con->db[jeho::db::table_name<T>() + "." + sub_db];
   col.drop();
  }

  template<typename T>
  void remove_table(std::shared_ptr<connection>  con )
  {
    auto col = con->db[jeho::db::table_name<T>()];
   col.drop();
  }


}


