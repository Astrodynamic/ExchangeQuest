#include <iostream>

#include "server.h"

int main(int argc, char* argv[]) {
  try {
    boost::asio::io_context io_context;
    Server server(io_context, std::atoi(argv[2]));
    io_context.run();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
