#include <iostream>

#include "client.h"

int main(int argc, char* argv[]) {
  try {
    boost::asio::io_context io_context;
    Client client(io_context, "localhost", std::atoi(argv[1]));
    // код для взаимодействия клиента с сервером

  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
