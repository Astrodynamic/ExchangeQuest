#include <boost/asio.hpp>

class Server {
 public:
  Server(boost::asio::io_context& io_context, short port);

 private:
};
