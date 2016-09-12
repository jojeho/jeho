#pragma once
#include "in.hpp"
#include "meta.hpp"
#include "csv_common.hpp"

namespace jeho { namespace io {

    namespace csv_detail
    {
      bool is_comment(std::string const& line)
      {
	return line.find("#") == 0;
      }

    }
    
    template <typename T>
    std::enable_if_t<std::is_same<T , std::string>::value  , void>
    from_csv(std::istream &in , T &t)
    {
      char token=',';
      char c;
      while (in.get(c) && c != token) {
        t += c;
      }

      int length = in.tellg();
      in.seekg(length - 1 , in.beg);
    }


    template <typename T>
    std::enable_if_t<is_std_array<T>::value  , void>
    from_csv(std::istream &ar , T &t)
    {
      char token;
      for(int i = 0 ; i < t.size(); ++i)
	{
	  ar>>t[i];
	  ar>>token;
	}

      int length = ar.tellg();
      ar.seekg(length - 1 , ar.beg);
    }
    
    template <typename T>
    std::enable_if_t<!boost::hana::Struct<T>::value
		     && !std::is_same<typename boost::hana::tag_of<T>::type
				      ,boost::hana::tag_of<boost::hana::tuple<>>::type>::value
		     && !is_vector<T>::value
		     && !std::is_same<T ,std::string>::value
		     && !is_std_array<T>::value
		     ,void>
    from_csv(std::istream &ar ,T & t)
    {
      ar>>t;
    };


    template <typename T>
    std::enable_if_t<boost::hana::Struct<T>::value,void>
    from_csv(std::istream & ifs  , T &t)
    {
      char token;
      int index =0;
      boost::hana::for_each(boost::hana::keys(t) , [&](auto b)
			    {
			      auto& member = boost::hana::at_key(t ,b);
			      using Member = std::remove_reference_t<decltype(member)>;
			      if(index !=0 )
				{
				  ifs>>token;
				  //std::cout<<" toke "<<token<<std::endl;
				}
			      index++;
			      from_csv<Member>(ifs , member);
			      //std::cout<<member<<std::endl;
			    });
    }


    template <typename T>
    std::enable_if_t<std::is_same<typename boost::hana::tag_of<T>::type
				  ,boost::hana::tag_of<boost::hana::tuple<>>::type>::value, void>
    from_csv(std::istream &ifs , T &t)
    {
      int index =0;
      char token;
      boost::hana::for_each(t , [&](auto & member)
			    {
			      //auto& member = boost::hana::at_key(t ,b);
			      using Member = std::remove_reference_t<decltype(member)>;

			      if(index !=0 )
				{
				  ifs>>token;
				  //std::cout<<" toke "<<token<<std::endl;
				}
			      index++;
			      
			      from_csv<Member>(ifs,member);
			    });
    }

    template <typename T>
    std::enable_if_t<is_vector<T>::value,void>
    from_csv(std::istream & ifs, T &t)
    {
      std::string line;
      while(std::getline(ifs, line))
      	{
	  if(!csv_detail::is_comment(line))
	    {
	      std::stringstream is(line);
	      typename T::value_type v;
	      from_csv<typename T::value_type>(is ,v);
	      t.push_back(v);
	    }
      	}
    }


    template <typename T>
    std::enable_if_t<is_vector<T>::value,void>
    from_csv(std::list<std::string> const& lines, T &t)
    {
      std::string line;
      for(auto const& line : lines)
      	{
	  if(!csv_detail::is_comment(line))
	    {
	      std::stringstream is(line);
	      typename T::value_type v;
	      from_csv<typename T::value_type>(is ,v);
	      t.push_back(v);
	    }
      	}

      std::cout<<t.size()<<std::endl;
    }

    
    template<>
    struct in<csv>
    {
      template<typename T>
      in(std::string const& file_name,T &t,bool header)
      {
	std::ifstream ifs(file_name.c_str());

	if(!ifs.good())
	  {
	    std::cout<<"no file "<<file_name<<std::endl;
	  }
	
	if(header == true)
	  {
	    std::string line;
	    std::list<std::string> lines;
	    bool first = true;
	    while(std::getline(ifs , line))
	      {
		if(!csv_detail::is_comment(line))
		  {
		    //std::cout<<line<<std::endl;
		    if(first)
		      {
			first= false;
		      }
		    else
		      {
			lines.push_back(line);
		      }
		  }
	      }
	    from_csv<T>(lines,t);
	  }
	else
	  {
	    std::stringstream is;
	    is<<ifs.rdbuf();
	    from_csv<T>(is , t);
	  }
      }
    };



  }}
