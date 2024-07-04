#include <boost/asio.hpp>

class Client {
 public:
  Client(boost::asio::io_context& io_context, const std::string& host, short port);

 private:
};
