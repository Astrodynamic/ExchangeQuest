#include <iostream>

#include "client.h"

int main() {
  try {
    boost::asio::io_context context;
    Client client(context);
    client.AsyncConnect(common::kIP.data(), std::to_string(common::kPort));

    command::Data regCmd;
    regCmd.type = command::Type::kRegistrationRequest;
    client.Send(regCmd);

    context.run();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return EXIT_SUCCESS;
}
