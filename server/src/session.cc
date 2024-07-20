#include "session.h"

#include <iostream>
#include <boost/bind/bind.hpp>

#include "command.h"

Session::Session(boost::asio::ip::tcp::socket socket, Exchange& exchange)
    : m_socket(std::move(socket)), m_exchange(exchange) {
  InitializeHandlers();
}

void Session::Start() {
  AsyncRead();
}

void Session::InitializeHandlers() {
  m_handlers[command::Type::kRegistrationRequest] = [this](const command::Data& command) {
    command::Data response;
    response.type = command::Type::kRegistrationResponse;
    response.data.registration_response.status = true;
    response.data.registration_response.uid = m_exchange.Registration();
    m_handlers[response.type](response);
  };

  m_handlers[command::Type::kRegistrationResponse] = [this](const command::Data& command) {
    std::memcpy(m_buffer_w.data(), &command, sizeof(command::Data));
    AsyncWrite();
  };

  m_handlers[command::Type::kLoginRequest] = [this](const command::Data& command) {
    command::Data response;
    response.type = command::Type::kLoginResponse;
    response.data.login_response.status = m_exchange.Login(command.data.login_request.uid);
    m_handlers[response.type](response);
  };

  m_handlers[command::Type::kLoginResponse] = [this](const command::Data& command) {
    std::memcpy(m_buffer_w.data(), &command, sizeof(command::Data));
    AsyncWrite();
  };

  m_handlers[command::Type::kOrderRequest] = [this](const command::Data& command) {
    m_exchange.Order(order::Data {
      .UID = command.UID,
      .instrument = command.data.order_request.instrument,
      .amount = command.data.order_request.amount,
      .price = command.data.order_request.price,
      .type = command.data.order_request.type,
      .timestamp = command.timestamp,
      .command = [this](order::Data data) {
        command::Data response {
          .UID = data.UID,
          .type = command::Type::kOrderResponce,
          .data = {
            . order_response {
              .instrument = data.instrument,
              .amount = data.amount,
              .price = data.price,
              .type = data.type
            }
          },
          .timestamp = data.timestamp
        };
        m_handlers[response.type](response);
      }
    });
  };

  m_handlers[command::Type::kOrderResponce] = [this](const command::Data& command) {
    std::memcpy(m_buffer_w.data(), &command, sizeof(command::Data));
    AsyncWrite();
  };

  m_handlers[command::Type::kBalansRequest] = [this](const command::Data& command) {
    command::Data response;
    response.type = command::Type::kBalansResponse;
    response.data.balans_response.status = true;
    response.data.balans_response.balance = m_exchange.Balance(command.data.balans_request.uid);
    m_handlers[response.type](response);
  };

  m_handlers[command::Type::kBalansResponse] = [this](const command::Data& command) {
    std::memcpy(m_buffer_w.data(), &command, sizeof(command::Data));
    AsyncWrite();
  };
}

void Session::AsyncRead() {
  m_socket.async_receive(
    boost::asio::buffer(m_buffer_r), boost::bind(
      &Session::onRead,
      shared_from_this(),
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred
    )
  );
}

void Session::AsyncWrite() {
  m_socket.async_send(
    boost::asio::buffer(m_buffer_w), boost::bind(
      &Session::onWrite,
      shared_from_this(),
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred
    )
  );
}

void Session::onRead(boost::system::error_code error, std::size_t bytes) {
  if (!error && sizeof(command::Data) >= bytes) {
    command::Data command;
    std::memcpy(&command, m_buffer_r.data(), sizeof(command::Data));
    m_handlers[command.type](command);
  } else if (error == boost::asio::error::eof) {
    std::cerr << "Connection closed by peer" << '\n';
  } else {
    std::cerr << error.message() << '\n';
  }
}

void Session::onWrite(boost::system::error_code error, std::size_t bytes) {
  if (!error && bytes >= sizeof(command::Data)) {
    AsyncRead();
  } else if (error == boost::asio::error::eof) {
    std::cerr << "Connection closed by peer" << '\n';
  } else {
    std::cerr << error.message() << '\n';
  }
}

