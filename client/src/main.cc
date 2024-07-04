#include <iostream>

#include "client.h"

int main(int argc, char* argv[]) {
  try {
    boost::asio::io_context io_context;
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
