#pragma once
#include <boost/hana.hpp>

namespace jeho { namespace network {
template<typename Func,int>
struct execute
{};

template<typename Func>
struct execute<Func ,1>
{
  template<typename Arg>
  auto operator()(Arg const&arg)->decltype(Func()(boost::hana::at_c<0>(arg)))
  {
    return Func()(boost::hana::at_c<0>(arg));
  }
};

template<typename Func>
struct execute<Func ,2>
{
  template<typename Args>
  auto operator()(Args const& arg)->decltype(Func()(boost::hana::at_c<0>(arg)
						    ,boost::hana::at_c<1>(arg)))
  {

    return Func()(boost::hana::at_c<0>(arg)
		  ,boost::hana::at_c<1>(arg));
  }
};


template<typename Func>
struct execute<Func ,3>
{
  template<typename Args>
  auto operator()(Args const& arg)->decltype(Func()(boost::hana::at_c<0>(arg)
						    ,boost::hana::at_c<1>(arg)
    						    ,boost::hana::at_c<2>(arg)))
  {

    return Func()(boost::hana::at_c<0>(arg)
		  ,boost::hana::at_c<1>(arg)
    		  ,boost::hana::at_c<2>(arg));
  }
};



template<typename Func>
struct execute<Func ,4>
{
  template<typename Args>
  auto operator()(Args const& arg)->decltype(Func()(boost::hana::at_c<0>(arg)
						    ,boost::hana::at_c<1>(arg)
						    ,boost::hana::at_c<2>(arg)
						    ,boost::hana::at_c<3>(arg)))
						    
  {

    return Func()(boost::hana::at_c<0>(arg)
		  ,boost::hana::at_c<1>(arg)
		  ,boost::hana::at_c<2>(arg)
    		  ,boost::hana::at_c<3>(arg));
  }
};



template<typename Func>
struct execute<Func ,5>
{
  template<typename Args>
  auto operator()(Args const& arg)->decltype(Func()(boost::hana::at_c<0>(arg)
						    ,boost::hana::at_c<1>(arg)
						    ,boost::hana::at_c<2>(arg)
						    ,boost::hana::at_c<3>(arg)
						    ,boost::hana::at_c<4>(arg)))
						    
  {

    return Func()(boost::hana::at_c<0>(arg)
		  ,boost::hana::at_c<1>(arg)
		  ,boost::hana::at_c<2>(arg)
		  ,boost::hana::at_c<3>(arg)
    		  ,boost::hana::at_c<4>(arg));
  }
};
  }}
