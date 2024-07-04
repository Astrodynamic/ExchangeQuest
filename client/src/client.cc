#include "client.h"

Client::Client(boost::asio::io_context& io_context, const std::string& host, short port)
    : socket_(io_context) {
  boost::asio::ip::tcp::resolver resolver(io_context);
  auto endpoints = resolver.resolve(host, std::to_string(port));
  boost::asio::connect(socket_, endpoints);
}

void Client::send_order(const Order& order) {
  nlohmann::json json_order = {
      {"client_id", order.client_id},
      {"amount", order.amount},
      {"price", order.price},
      {"side", order.side}};
  write_msg_ = json_order.dump();
  boost::asio::async_write(socket_, boost::asio::buffer(write_msg_),
                           [this](boost::system::error_code ec, std::size_t /*length*/) {
                             handle_write(ec, /*length*/);
                           });
}

void Client::check_balance() {
  // код для запроса баланса клиента
}

void Client::handle_write(const boost::system::error_code& error, std::size_t bytes_transferred) {
  if (!error) {
    // код для обработки успешной отправки данных
  }
}

void Client::handle_read(const boost::system::error_code& error, std::size_t bytes_transferred) {
  if (!error) {
    // код для обработки успешного чтения данных
  }
}
