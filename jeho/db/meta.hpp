#pragma once

namespace jeho { namespace db {

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
    

  }}
