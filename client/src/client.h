#pragma once

#include <optional>
#include <thread>

#include <boost/asio.hpp>

#include "command.h"

class Client {
 public:
  Client();
  ~Client();

  auto AsyncConnect(const std::string& host, const std::string& port) -> void;
  auto Connected() const -> bool;
  auto Logined() const -> bool;

  auto Send(command::Data& command) -> bool;

 private:
  auto InitializeHandlers() -> void;
  auto AsyncRead() -> void;
  auto AsyncWrite() -> void;

  auto onConnect(const boost::system::error_code& error) -> void;
  auto onRead(const boost::system::error_code& error, std::size_t bytes) -> void;
  auto onWrite(const boost::system::error_code& error, std::size_t bytes) -> void;

  std::unordered_map<command::Type, std::function<void(const command::Data&)>> m_handlers;

  std::array<std::byte, sizeof(command::Data)> m_buffer_r{};
  std::array<std::byte, sizeof(command::Data)> m_buffer_w{};

  boost::asio::io_context m_context;
  boost::asio::ip::tcp::socket m_socket;
  boost::asio::ip::tcp::resolver m_resolver;

  bool m_connected{false};
  common::uid_t m_UID{static_cast<common::uid_t>(common::UIDType::kCommon)};
  std::optional<std::thread> m_context_thread;
};