#pragma once

#include <boost/asio.hpp>
#include <optional>

#include "exchange.h"

class Server {
 public:
  Server(boost::asio::io_context& context, std::uint16_t port);

  auto AsyncAccept() -> void;

 private:
  boost::asio::io_context& m_context;
  boost::asio::ip::tcp::acceptor m_acceptor;
  std::optional<boost::asio::ip::tcp::socket> m_socket;
  Exchange m_exchange;
};
