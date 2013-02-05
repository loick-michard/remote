#ifndef _SERVER_HPP_
# define _SERVER_HPP_

# include <boost/asio.hpp>
# include "Client.hpp"

using namespace boost::asio::ip;

class Server {
	public:
		Server(boost::asio::io_service&, int port = 4242);
		~Server();

		void acceptHandler(const boost::system::error_code& error);
		void tcpWriteToAll(int size, void* data);
		void tcpDataReceived(int size, char* data);
	private:	
		boost::asio::io_service		*_ioService;
		tcp::acceptor				_tcpAcceptor;
		std::vector<Client*>		_clients;
};

#endif