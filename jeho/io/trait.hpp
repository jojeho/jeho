#pragma once

namespace jeho { namespace io {

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

  }}
