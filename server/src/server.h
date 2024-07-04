#include <boost/asio.hpp>
#include <mutex>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>

class Server {
 public:
  Server(boost::asio::io_context& io_context, short port);
  void start_accept();
  void handle_accept(const boost::system::error_code& error);

 private:
  void process_request(const std::string& request, std::string& response);
  void match_orders();

  boost::asio::ip::tcp::acceptor acceptor_;
  std::unordered_map<int, std::pair<double, double>> balances_;  // ClientID -> (USD, RUB)
  std::vector<Order> buy_orders_;
  std::vector<Order> sell_orders_;
  std::mutex mtx_;
};
