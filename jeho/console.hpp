#pragma once
#include <stdio.h>
#include <iostream>
namespace jeho { namespace console {

    void set_title(std::string title)
    {
      printf("%c]0;%s%c", '\033', title.c_str(), '\007');
    }

    void pause(std::string msg="continue?")
    {
      std::cout<<msg;
      std::cin>>msg;
    }

    template<typename T>
    T input(std::string const& msg)
    {
      std::cout<<msg<<":";
      T t;
      std::cin>>t;
      return t;
    }
  }
}
