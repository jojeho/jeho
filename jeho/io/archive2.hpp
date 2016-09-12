#pragma once
#include "meta.hpp"
#include "chrono.hpp"
#include <type_traits>
#include <list>
#include <boost/serialization/list.hpp>

namespace jeho { namespace io {

    template <typename T> struct is_container {
      static bool const value = false;  
    };

    template <typename... Args>
    struct is_container<std::list<Args...>> {
      static bool const value = true;
    };

    template <typename... Args>
    struct is_container<std::vector<Args...>> {
      static bool const value = true;
    };

    
    template <typename Stream ,typename T>
    std::enable_if_t<!boost::hana::Struct<T>::value
		     && !std::is_same<typename boost::hana::tag_of<T>::type
				      ,boost::hana::tag_of<boost::hana::tuple<>>::type>::value
		     && !is_container<T>::value
		     ,void>
    out_from(Stream &ar ,T const& in)
    {
      ar<<in;
    };

    template <typename Stream ,typename T>
    std::enable_if_t<std::is_same<typename boost::hana::tag_of<T>::type
				  ,boost::hana::tag_of<boost::hana::tuple<>>::type>::value, void>
    out_from(Stream &ar ,T const& in)
    {
      boost::hana::for_each(in , [&](auto b)
			    {
			      using Member = std::remove_reference_t<decltype(b)>;
			      out_from<Stream,Member>(ar,b);
			    });

    };


    
    template <typename Stream,typename T>
    std::enable_if_t<boost::hana::Struct<T>::value,void>
    out_from(Stream &ar ,T const& in)
    {
      boost::hana::for_each(boost::hana::keys(in), [&](auto  b)
			    {
			      auto& member = boost::hana::at_key(in ,b);
			      using Member = std::remove_reference_t<decltype(member)>;
			      out_from<Stream,Member>(ar,member);
			    });
    }

    template <typename Stream ,typename T>
    std::enable_if_t<is_container<T>::value, void>
    out_from(Stream &ar ,T const& in)
    {
      std::for_each(std::begin(in) , std::end(in),[&](auto const&i)
		    {
		      using Member = std::remove_reference_t<decltype(i)>;
		      out_from<Stream ,Member>(ar,i);
		    });
      // boost::hana::for_each(in , [&](auto b)
      // 			    {
      // 			      using Member = std::remove_reference_t<decltype(b)>;
      // 			      out_from<Stream,Member>(ar,b);
      // 			    });

    };



    /////////////////in

    

    
    template <typename Stream ,typename T>
    std::enable_if_t<!boost::hana::Struct<T>::value
		     && !std::is_same<typename boost::hana::tag_of<T>::type
				      ,boost::hana::tag_of<boost::hana::tuple<>>::type>::value
		     && !is_container<T>::value
		     ,void>
    in_to(Stream &ar ,T & t)
    {
      ar>>t;
    };


    template <typename Stream ,typename T>
    std::enable_if_t<std::is_same<typename boost::hana::tag_of<T>::type
				  ,boost::hana::tag_of<boost::hana::tuple<>>::type>::value, void>
    in_to(Stream &ar , T &t)
    {

      boost::hana::for_each(t , [&](auto b)
			    {
			      using Member = std::remove_reference_t<decltype(b)>;
			      in_to<Stream,Member>(ar,b);
			    });
    }



    
    template <typename Stream,typename T>
    std::enable_if_t<boost::hana::Struct<T>::value,void>
    in_to(Stream &ar ,T & t)
    {
      boost::hana::for_each(boost::hana::keys(t), [&](auto  b)
			    {
			      auto& member = boost::hana::at_key(t ,b);
			      using Member = std::remove_reference_t<decltype(member)>;
			      in_to<Stream,Member>(ar,member);
			    });
    }


    template <typename Stream ,typename T>
    std::enable_if_t<is_container<T>::value, void>
    in_to(Stream &ar ,T & t)
    {
      //ar>>in;
      // std::for_each(std::begin(in) , std::end(in),[&](auto &i)
      // 		    {
      // 		      using Member = std::remove_reference_t<decltype(i)>;
      // 		      in_to<Stream,Member>(ar ,i);//ar>>i;
      // 		    });

    };


  }}
