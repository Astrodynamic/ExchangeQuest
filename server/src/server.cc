#include "server.h"

#include "common.h"

Server::Server(boost::asio::io_context& io_context, short port)
    : acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
  start_accept();
}

void Server::start_accept() {
  // код для принятия новых подключений
}

void Server::handle_accept(const boost::system::error_code& error) {
  if (!error) {
    // код для обработки нового соединения
  }
  start_accept();
}

void Server::process_request(const std::string& request, std::string& response) {
  nlohmann::json json_request = nlohmann::json::parse(request);
  // код для обработки запроса и формирования ответа
}

void Server::match_orders() {
  std::lock_guard<std::mutex> lock(mtx_);
  // код для логики матчинга заявок
}
