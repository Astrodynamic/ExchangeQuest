#include <boost/asio.hpp>
#include <iostream>
#include "common.h"
#include "command.h"

using boost::asio::ip::tcp;

void sendOrder(tcp::socket& socket, const std::string& message) {
  boost::asio::write(socket, boost::asio::buffer(message + "\n"));
}

int main() {
  try {
    boost::asio::io_context io_context;
    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(common::kIP.data(), std::to_string(common::kPort));
    tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);

    command::Data command;
    command.type = command::Type::kRegistrationRequest;

    std::array<std::byte, sizeof(command::Data)> buffer;
    std::memcpy(buffer.data(), &command, sizeof(command::Data));

    socket.async_send(
      boost::asio::buffer(buffer), [&](boost::system::error_code ec, std::size_t bytes) {
        std::cout << "Sent" << std::endl;
      }
    );

    socket.async_receive(
      boost::asio::buffer(buffer), [&](boost::system::error_code ec, std::size_t bytes) {
        std::cout << "Received" << std::endl;

        command::Data response;
        std::memcpy(&response, buffer.data(), sizeof(command::Data));
        if (response.type == command::Type::kRegistrationResponse) {
          std::cout << "kOrderResponce" << std::endl;

          std::cout << "UID: " << response.data.registration_response.uid << std::endl;
        }
      }
    );

    io_context.run();
  } catch (std::exception& e) {
    std::cerr << e.what() << "\n";
  }

  return 0;
}

