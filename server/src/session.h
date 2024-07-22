#pragma once

#include <boost/asio.hpp>

#include "exchange.h"
#include "command.h"

class Session : public std::enable_shared_from_this<Session> {
 public:
  Session(boost::asio::ip::tcp::socket socket, Exchange& exchange);

  auto Start() -> void;

 private:
  auto InitializeHandlers() -> void;
  auto AsyncRead() -> void;
  auto AsyncWrite() -> void;

  auto onRead(boost::system::error_code error, std::size_t bytes) -> void;
  auto onWrite(boost::system::error_code error, std::size_t bytes) -> void;

  std::unordered_map<command::Type, std::function<void(const command::Data&)>> m_handlers;

  std::array<std::byte, sizeof(command::Data)> m_buffer_r{};
  std::array<std::byte, sizeof(command::Data)> m_buffer_w{};

  boost::asio::ip::tcp::socket m_socket;
  Exchange& m_exchange;

};