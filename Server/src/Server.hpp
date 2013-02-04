#ifndef _SERVER_HPP_
# define _SERVER_HPP_

#include <boost/asio.hpp>

using namespace boost::asio::ip;

class Server {
	public:
		Server(boost::asio::io_service&, int port = 4242);
		~Server();

		void acceptHandler(const boost::system::error_code& error);
	private:	
		boost::asio::io_service		*_ioService;
		tcp::acceptor				_tcpAcceptor;
		std::vector<tcp::socket*>	_clients;
};

#endif