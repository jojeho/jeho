#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>


namespace jeho { namespace network {

class req_protocol
{
public:
  enum { header_length = sizeof(long) };
};
    
}}
