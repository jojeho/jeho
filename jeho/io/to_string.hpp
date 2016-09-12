#pragma once
#include <chrono>

namespace jeho { namespace io {

    
    std::string to_string(std::chrono::system_clock::time_point const&tp)
    {
      std::time_t tx = std::chrono::system_clock::to_time_t(tp);
      return std::ctime(&tx);
    }

  }}
