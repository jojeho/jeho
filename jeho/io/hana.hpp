#pragma once
#include <boost/hana.hpp>

namespace jeho {

auto out = [](std::ostream& os, auto const& object) {

  using namespace boost;
  boost::hana::for_each(object, hana::fuse([](auto name, auto member) {
        std::cout << hana::to<char const*>(name) << ": "
                  << member << std::endl;
      }));

  std::cout<<std::endl;
};

auto out2 = [](std::ostream& os, auto const& object) {

  boost::hana::for_each(boost::hana::keys(object) , [&](auto  key)
  {
    using namespace std;
    using namespace  boost;
    os<<hana::to<char const*>(key)<<":"
    <<hana::at_key(object , key)<<" ";
  });
  std::cout<<std::endl;
};

  struct c_out{};

  template<typename T>
  c_out& operator<<(c_out &o , T const&t)
  {
    out(std::cout , t);
    return o;
  }
  
  c_out cout;
}
