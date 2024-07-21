#include "client.h"

#include <boost/bind/bind.hpp>
#include <iostream>

Client::Client(boost::asio::io_context& context)
    : m_socket(context), m_resolver(context) {
  InitializeHandlers();
}

auto Client::AsyncConnect(const std::string& host, const std::string& port) -> void {
  boost::asio::async_connect(m_socket, m_resolver.resolve(host, port),
                             boost::bind(&Client::onConnect, this, boost::asio::placeholders::error));
}

auto Client::Connected() const -> bool {
  return m_socket.is_open();
}

auto Client::Close() -> void {
  if (m_socket.is_open()) {
    m_socket.close();
  }
}

auto Client::Send(const command::Data& command) -> bool {
  if (!m_socket.is_open()) {
    std::cerr << "Socket is not open." << std::endl;
    return false;
  }

  std::memcpy(m_buffer_w.data(), &command, sizeof(command));
  AsyncWrite();
  return true;
}

auto Client::InitializeHandlers() -> void {
  m_handlers[command::Type::kRegistrationResponse] = [this](const command::Data& command) {
    std::cout << "Registration successful." << std::endl;
  };
}

auto Client::AsyncRead() -> void {
  if (!m_socket.is_open()) {
    std::cerr << "Socket is not open." << std::endl;
    return;
  }

  m_socket.async_receive(
      boost::asio::buffer(m_buffer_r), boost::bind(
                                           &Client::onRead,
                                           this,
                                           boost::asio::placeholders::error,
                                           boost::asio::placeholders::bytes_transferred));
}

auto Client::AsyncWrite() -> void {
  if (!m_socket.is_open()) {
    std::cerr << "Socket is not open." << std::endl;
    return;
  }

  m_socket.async_send(
      boost::asio::buffer(m_buffer_w), boost::bind(
                                           &Client::onWrite,
                                           this,
                                           boost::asio::placeholders::error,
                                           boost::asio::placeholders::bytes_transferred));
}

auto Client::onConnect(const boost::system::error_code& error) -> void {
  if (!error) {
    std::cout << "Successfully connected to the server." << std::endl;
    AsyncRead();
  } else {
    std::cerr << "Connect error: " << error.message() << std::endl;
  }
}

auto Client::onRead(const boost::system::error_code& error, std::size_t bytes) -> void {
  if (!error && bytes >= sizeof(command::Data)) {
    command::Data command;
    std::memcpy(&command, m_buffer_r.data(), sizeof(command::Data));
    m_handlers[command.type](command);
  } else if (error == boost::asio::error::eof) {
    std::cerr << "Connection closed by peer" << '\n';
  } else {
    std::cerr << error.message() << '\n';
  }
}

auto Client::onWrite(const boost::system::error_code& error, std::size_t bytes) -> void {
  if (!error && bytes >= sizeof(command::Data)) {
    AsyncRead();
  } else if (error == boost::asio::error::eof) {
    std::cerr << "Connection closed by peer" << '\n';
  } else {
    std::cerr << error.message() << '\n';
  }
}