#include "client.h"

#include <iostream>

#include <boost/bind/bind.hpp>

#include "common.h"

Client::Client() : m_socket(m_context), m_resolver(m_context) {
  InitializeHandlers();
}

Client::~Client() {
  Close();
  if (m_context_thread) {
    m_context.stop();
    m_context_thread->join();
  }
}

auto Client::AsyncConnect(const std::string& host, const std::string& port) -> void {
  boost::asio::async_connect(m_socket, m_resolver.resolve(host, port), boost::bind(&Client::onConnect, this, boost::asio::placeholders::error));
  m_context_thread = std::thread([this]() {
    m_context.run();
  });
}

auto Client::Connected() const -> bool {
  return m_socket.is_open();
}

auto Client::Close() -> void {
  boost::asio::post(m_socket.get_executor(), [this]() {
    if (Connected()) {
      m_socket.close();
    }
  });
}

auto Client::Send(const command::Data& command) -> bool {
  if (!Connected()) {
    std::cerr << "Socket is not open." << std::endl;
    return false;
  }

  std::memcpy(m_buffer_w.data(), &command, sizeof(command));
  AsyncWrite();
  return true;
}

auto Client::InitializeHandlers() -> void {
  m_handlers[command::Type::kRegistrationRequest] = [this](const command::Data& command) {
    std::memcpy(m_buffer_w.data(), &command, sizeof(command::Data));
    AsyncWrite();
  };

  m_handlers[command::Type::kRegistrationResponse] = [this](const command::Data& command) {
    std::cout << "UID: " << command.UID << std::endl;
  };

  m_handlers[command::Type::kLoginRequest] = [this](const command::Data& command) {
    std::memcpy(m_buffer_w.data(), &command, sizeof(command::Data));
    AsyncWrite();
  };

  m_handlers[command::Type::kLoginResponse] = [this](const command::Data& command) {
    
  };

  m_handlers[command::Type::kOrderRequest] = [this](const command::Data& command) {
    std::memcpy(m_buffer_w.data(), &command, sizeof(command::Data));
    AsyncWrite();
  };

  m_handlers[command::Type::kOrderResponce] = [this](const command::Data& command) {
    
  };

  m_handlers[command::Type::kBalansRequest] = [this](const command::Data& command) {
    std::memcpy(m_buffer_w.data(), &command, sizeof(command::Data));
    AsyncWrite();
  };

  m_handlers[command::Type::kBalansResponse] = [this](const command::Data& command) {

  };
}

auto Client::AsyncRead() -> void {
  if (!Connected()) {
    std::cerr << "Socket is not open." << std::endl;
    return;
  }

  m_socket.async_receive(boost::asio::buffer(m_buffer_r), boost::bind(&Client::onRead, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

auto Client::AsyncWrite() -> void {
  if (!Connected()) {
    std::cerr << "Socket is not open." << std::endl;
    return;
  }

  m_socket.async_send(boost::asio::buffer(m_buffer_w), boost::bind(&Client::onWrite, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
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
    if (auto handler = m_handlers.find(command.type); handler != m_handlers.end()) {
      handler->second(command);
    } else {
      std::cerr << "Unknown command." << std::endl;
    }
    AsyncRead();
  } else if (error == boost::asio::error::eof) {
    std::cerr << "Connection closed by peer" << '\n';
  } else {
    std::cerr << error.message() << '\n';
  }
}

auto Client::onWrite(const boost::system::error_code& error, std::size_t bytes) -> void {
  if (!error && bytes >= sizeof(command::Data)) {
    std::cout << "Command sent." << std::endl;
  } else if (error == boost::asio::error::eof) {
    std::cerr << "Connection closed by peer" << '\n';
  } else {
    std::cerr << error.message() << '\n';
  }
}