#include "database/database.h"
#include "database/settings.h"
#include "message_handler/MessageHandler.h"
#include "person/person.h"
#include "sqlite3/sqlite3.h"
#include "json/json-forwards.h"
#include "json/json.h"
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/ref.hpp>
#include <csignal>
#include <iostream>
#include <memory>
#include <string>

using namespace boost;

class tcp_connection : public boost::enable_shared_from_this<tcp_connection> {
public:
  typedef boost::shared_ptr<tcp_connection> pointer;

  static pointer create(boost::asio::io_service &io_service) {
    return pointer(new tcp_connection(io_service));
  }

  asio::ip::tcp::socket &socket() { return socket_; }

  void start(const std::string &received_message, Database &data) {
    std::cout << "RECEIVED: " << received_message;

    MessageHandler handler(received_message, data);

    message_ = handler.GetResponse();
    boost::asio::async_write(
        socket_, boost::asio::buffer(message_),
        boost::bind(&tcp_connection::handle_write, shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
  }

private:
  tcp_connection(boost::asio::io_service &io_service) : socket_(io_service) {}

  void handle_write(const boost::system::error_code & /*error*/,
                    size_t /*bytes_transferred*/) {}

  asio::ip::tcp::socket socket_;
  std::string message_;
};

class tcp_server {
public:
  tcp_server(boost::asio::io_service &io_service)
      : acceptor_(io_service,
                  asio::ip::tcp::endpoint(
                      asio::ip::address_v4::from_string("127.0.0.1"), 8888)),
        database_(DATABASE) {
    start_accept();
  }

  void CloseTcpServer() {
    std::cout << "\nClosing the server\n";
    database_.SaveData();
  }

private:
  void start_accept() {
    tcp_connection::pointer new_connection = tcp_connection::create(
        (boost::asio::io_context &)acceptor_.get_executor().context());
    acceptor_.async_accept(new_connection->socket(),
                           boost::bind(&tcp_server::handle_accept, this,
                                       new_connection,
                                       boost::asio::placeholders::error));
  }

  void handle_accept(tcp_connection::pointer new_connection,
                     const boost::system::error_code &error) {
    strcpy(received_message_, "");
    if (!error) {
      new_connection->socket().async_read_some(
          boost::asio::buffer(received_message_, 4096),
          boost::bind(&tcp_connection::start, new_connection, received_message_,
                      boost::ref(database_)));
    }

    start_accept();
  }

  char received_message_[4096];
  asio::ip::tcp::acceptor acceptor_;
  Database database_;
};

tcp_server *server_ptr = nullptr;

void signal_handler(int) {
  if (server_ptr != nullptr) {
    server_ptr->CloseTcpServer();
  }

  exit(0);
}

int main() {
  boost::asio::io_service io_service;
  tcp_server server(io_service);
  server_ptr = &server;

  std::signal(SIGINT, signal_handler);

  try {
    io_service.run();
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}