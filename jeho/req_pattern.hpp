#pragma once

namespace jeho {

  template<typename Func, typename T,typename Constrain>
  struct req_res
  {};

  template<typename Func, typename Constrain>
  struct req_void
  {};

  template<typename Func, typename T,typename Constrain>
  struct req_up
  {};
  
}
