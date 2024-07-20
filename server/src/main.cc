#include <iostream>

#include "server.h"

int main() {
  try {
    boost::asio::io_context context;
    Server server(context, common::kPort);
    server.AsyncAccept();
    context.run();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return EXIT_SUCCESS;
}
