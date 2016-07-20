#pragma once
#include "meta.hpp"
#include "chrono.hpp"
#include <type_traits>
#include <list>
#include <map>
#include <vector>
#include <set>
//#include <boost/serialization/map.hpp>
//#include <boost/serialization/set.hpp>

namespace jeho { namespace stream {

    template <typename T> struct is_assoc {
      static bool const value = false;  
    };

    
    template <typename... Args>
    struct is_assoc<std::set<Args...>> {
      static bool const value = true;
    };

        
    template <typename... Args>
    struct is_assoc<std::map<Args...>> {
      static bool const value = true;
    };

    
    template <typename T> struct is_vector {
      static bool const value = false;  
    };

    template <typename... Args>
    struct is_vector<std::list<Args...>> {
      static bool const value = true;
    };

    template <typename... Args>
    struct is_vector<std::vector<Args...>> {
      static bool const value = true;
    };

    template <typename T> struct is_pair {
      static bool const value = false;  
    };

    template <typename... Args>
    struct is_pair<std::pair<Args...>> {
      static bool const value = true;
    };

    

    template <typename Stream ,typename T>
    std::enable_if_t<std::is_same<typename boost::hana::tag_of<T>::type
				  ,boost::hana::tag_of<boost::hana::tuple<>>::type>::value, Stream&>
    operator<<(Stream &ar ,T const& in)
    {
      boost::hana::for_each(in , [&](auto b)
			    {
			      using Member = std::remove_reference_t<decltype(b)>;
			      ///out_from<Stream,Member>(ar,b);
			      ar<<b;
			    });
      return ar;
    };

    
    template <typename Stream,typename T>
    std::enable_if_t<boost::hana::Struct<T>::value,Stream&>
    operator <<(Stream &ar ,T const& in)
    {
      boost::hana::for_each(boost::hana::keys(in), [&](auto  b)
			    {
			      auto& member = boost::hana::at_key(in ,b);
			      using Member = std::remove_reference_t<decltype(member)>;
			      ar<<member;
			    });
      return ar;
    }

    template <typename Stream ,typename T>
    std::enable_if_t<is_vector<T>::value, Stream&>
    operator <<(Stream &ar ,T const& in)
    {
      int size = in.size();
      ar<<size;
      std::for_each(std::begin(in),std::end(in) , [&](auto const& b)
      			    {
      			      ar<<b;
      			    });
      return ar;
    };


    template <typename Stream ,typename T>
    std::enable_if_t<is_pair<T>::value, Stream&>
    operator <<(Stream &ar ,T const& in)
    {
      // int size = in.size();
      // ar<<size;
      // std::for_each(std::begin(in),std::end(in) , [&](auto const& b)
      // 			    {
      // 			      ar<<b.first<<b.second;
      // 			    });
      return ar;
    };

    
    /////////////////in

    template <typename Stream ,typename T>
    std::enable_if_t<std::is_same<typename boost::hana::tag_of<T>::type
				  ,boost::hana::tag_of<boost::hana::tuple<>>::type>::value, Stream&>
    operator >>(Stream &ar , T &t)
    {
      boost::hana::for_each(t , [&](auto b)
			    {
			      using Member = std::remove_reference_t<decltype(b)>;
			      //in_to<Stream,Member>(ar,b);
			      ar>>b;
			    });
      return ar;
    }

    
    template <typename Stream,typename T>
    std::enable_if_t<boost::hana::Struct<T>::value,Stream&>
    operator >>(Stream &ar ,T & t)
    {
      
      boost::hana::for_each(boost::hana::keys(t), [&](auto  b)
			    {
			      auto& member = boost::hana::at_key(t ,b);
			      using Member = std::remove_reference_t<decltype(member)>;
			      ar>>member;
			    });
      return ar;
    }


    template <typename Stream ,typename T>
    std::enable_if_t<is_vector<T>::value , Stream&>
    operator >>(Stream &ar ,T & t)
    {
      int size ;
      ar>>size;
      for(int i = 0  ; i < size; ++i)
      	{
      	  typename T::value_type item;
      	  ar>>item;
      	  t.push_back(item);
      	}
      return ar;
    };


    template <typename Stream ,typename T>
    std::enable_if_t<is_pair<T>::value , Stream&>
    operator >>(Stream &ar ,T & t)
    {
      // int size ;
      // ar>>size;
      // for(int i = 0  ; i < size; ++i)
      // 	{
      // 	  typename T::value_type item;
      // 	  ar>>item.first;
      // 	  ar>>item.second;
      // 	  t.push_back(item);
      // 	}
      return ar;
    };


  }}
