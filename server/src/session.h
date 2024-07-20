#pragma once

#include <boost/asio.hpp>

#include "exchange.h"
#include "command.h"

class Session : public std::enable_shared_from_this<Session> {
 public:
  Session(boost::asio::ip::tcp::socket socket, Exchange& exchange);

  void Start();

 private:
  void InitializeHandlers();
  void AsyncRead();
  void AsyncWrite();

  void onRead(boost::system::error_code error, std::size_t bytes);
  void onWrite(boost::system::error_code error, std::size_t bytes);

  std::unordered_map<command::Type, std::function<void(const command::Data&)>> m_handlers;

  std::array<std::byte, sizeof(command::Data)> m_buffer_r;
  std::array<std::byte, sizeof(command::Data)> m_buffer_w;

  boost::asio::ip::tcp::socket m_socket;
  Exchange& m_exchange;

};