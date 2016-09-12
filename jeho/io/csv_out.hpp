#pragma once
#include "out.hpp"

namespace jeho { namespace io {

    template <typename T>
    std::enable_if_t<std::is_same<typename boost::hana::tag_of<T>::type
				  ,boost::hana::tag_of<boost::hana::tuple<>>::type>::value, void>
    to_csv(std::ostream &os , T const&t)
    {
      boost::hana::for_each(t , [&](auto b)
			    {
			      auto const& member = boost::hana::at_key(t ,b);
			      using Member = std::remove_reference_t<decltype(member)>;
			      to_csv<Member>(os,member);
			    });
    }

    template <typename T>
    std::enable_if_t<std::is_same<T , std::string>::value  , void>
    to_csv(std::ostream &os , T const&t)
    {
      os<<t;
    }

    template <typename T>
    std::enable_if_t<!boost::hana::Struct<T>::value
		     && !std::is_same<typename boost::hana::tag_of<T>::type
				      ,boost::hana::tag_of<boost::hana::tuple<>>::type>::value
		     && !is_vector<T>::value
		     && !std::is_same<T ,std::string>::value
		     ,void>
    to_csv(std::ostream &os ,T const& t)
    {
      os<<t;
    };


    template <typename T>
    std::enable_if_t<boost::hana::Struct<T>::value,void>
    to_csv(std::ostream & os  , T const&t)
    {
      char token;
      int index =0;
      boost::hana::for_each(boost::hana::keys(t), [&](auto  b)
			    {
			      auto const& member = boost::hana::at_key(t ,b);
			      using Member = std::remove_reference_t<decltype(member)>;
			      if(index !=0 )
				{
				  os<<",";
				}

			      index++;
			      to_csv<Member>(os , member);
			    });
    }

    template <typename T>
    std::enable_if_t<is_vector<T>::value,void>
    to_csv(std::ostream & os, T const&t)
    {
      std::string line;

      for(auto const&i : t)
	{
	  to_csv<decltype(i)>(os, i);
	  os<<std::endl;
	}
    }

    
    template<>
    struct out<csv>
    {
      template<typename T>
      out(std::string const& file_name, T const&t)
      {
	std::stringstream os;
	to_csv<T>(os , t);
	std::ofstream ofs(file_name.c_str());
	ofs<<os.rdbuf();
      }
    };

  }}
			   
