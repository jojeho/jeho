#include "jeho/network/req_res.hpp"
#include "config.hpp"
//using namespace jeho::network;

int main(int argc , char*argv[])
{
  // if (argc != 2)
  //   {
  //     std::cerr << "Usage: echo_server <port>\n";
  // 	return 1;
  //   }

  jeho::network::connection con(address , port);
  server  xbse(con);
}

