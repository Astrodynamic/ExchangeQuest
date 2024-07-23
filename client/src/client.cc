#include "client.h"

#include <iostream>
#include <chrono>

#include <boost/bind/bind.hpp>

#include "common.h"

Client::Client() : m_socket(m_context), m_resolver(m_context) {
  InitializeHandlers();
}

Client::~Client() {
  if (m_context_thread) {
    m_context.stop();
    m_context_thread->join();
  }
}

auto Client::AsyncConnect(const std::string& host, const std::string& port) -> void {
  if (!m_connected) {
    boost::asio::async_connect(m_socket, m_resolver.resolve(host, port), boost::bind(&Client::onConnect, this, boost::asio::placeholders::error));
    if (!m_context_thread) {
      m_context_thread = std::thread([this]() {
        m_context.run();
      });
    }
  } else {
    std::cerr << "Connection is already open." << std::endl;
  }
}

auto Client::Connected() const -> bool {
  return m_connected;
}

auto Client::Logined() const -> bool {
  return m_UID >= static_cast<common::uid_t>(common::UIDType::kClient);
}

auto Client::Send(command::Data& command) -> bool {
  command.UID = m_UID;
  command.timestamp = static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());

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
    std::cout << "Registration UID: " << command.data.registration_response.uid << std::endl;
  };

  m_handlers[command::Type::kLoginRequest] = [this](const command::Data& command) {
    std::memcpy(m_buffer_w.data(), &command, sizeof(command::Data));
    AsyncWrite();
  };

  m_handlers[command::Type::kLoginResponse] = [this](const command::Data& command) {
    if (command.data.login_response.status) {
      m_UID = command.data.login_response.UID;
      std::cout << "Login success. UID: " << m_UID << std::endl;
    } else {
      std::cout << "Login failed.\n";
    }
  };

  m_handlers[command::Type::kOrderRequest] = [this](const command::Data& command) {
    std::memcpy(m_buffer_w.data(), &command, sizeof(command::Data));
    AsyncWrite();
  };

  m_handlers[command::Type::kOrderResponce] = [this](const command::Data& command) {
    const auto& data = command.data.order_response;
    static std::unordered_map<std::uint16_t, std::string> instruments {
      {order::Instrument::RUB(), "RUB"},
      {order::Instrument::USD(), "USD"}
    };

    std::cout << "Order instrument: " << instruments[data.instrument.code] << std::endl;
    std::cout << "Order amount: " << data.amount << std::endl;
    std::cout << "Order price: " << data.price << std::endl;
    std::cout << "Order type: " << (data.type == order::Type::kBuy ? "BUY" : "SELL") << std::endl;
  };

  m_handlers[command::Type::kBalansRequest] = [this](const command::Data& command) {
    std::memcpy(m_buffer_w.data(), &command, sizeof(command::Data));
    AsyncWrite();
  };

  m_handlers[command::Type::kBalansResponse] = [this](const command::Data& command) {
    const auto& data = command.data.balans_response;
    static std::unordered_map<std::uint16_t, std::string> instruments {
      {order::Instrument::RUB(), "RUB"},
      {order::Instrument::USD(), "USD"}
    };

    for (const auto& instrument : data.balance) {
      std::cout << instruments[instrument.instrument] << " : " << instrument.amount << std::endl;
    }
  };
}

auto Client::AsyncRead() -> void {
  if (!Connected()) {
    std::cerr << "Connection is not open." << std::endl;
    return;
  }

  m_socket.async_receive(boost::asio::buffer(m_buffer_r), boost::bind(&Client::onRead, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

auto Client::AsyncWrite() -> void {
  if (!Connected()) {
    std::cerr << "Connection is not open." << std::endl;
    return;
  }

  m_socket.async_send(boost::asio::buffer(m_buffer_w), boost::bind(&Client::onWrite, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

auto Client::onConnect(const boost::system::error_code& error) -> void {
  if (!error) {
    m_connected = true;
    std::cout << "Successfully connected to the server." << std::endl;
    AsyncRead();
  } else {
    m_connected = false;
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