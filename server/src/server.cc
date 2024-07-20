#include "server.h"

#include "session.h"

Server::Server(boost::asio::io_context& context, std::uint16_t port)
    : m_context(context),
      m_acceptor(context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {}

void Server::AsyncAccept() {
  m_socket.emplace(m_context);
  m_acceptor.async_accept(*m_socket, [this](boost::system::error_code error) {
    if (!error) {
      std::make_shared<Session>(std::move(*m_socket), m_exchange)->Start();
    }
    AsyncAccept();
  });
}
