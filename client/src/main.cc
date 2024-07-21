#include <iostream>

#include "interface.h"

int main() {
  try {
    Client client;
    Interface interface(client);
    interface.Exec();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return EXIT_SUCCESS;
}
