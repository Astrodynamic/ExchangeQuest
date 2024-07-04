#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

class Client {
 public:
  Client(boost::asio::io_context& io_context, const std::string& host, short port);
  void send_order(const Order& order);
  void check_balance();

 private:
  void handle_write(const boost::system::error_code& error, std::size_t bytes_transferred);
  void handle_read(const boost::system::error_code& error, std::size_t bytes_transferred);

  boost::asio::ip::tcp::socket socket_;
  std::string read_msg_;
  std::string write_msg_;
};
