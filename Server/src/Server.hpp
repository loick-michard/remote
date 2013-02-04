#ifndef _SERVER_HPP_
# define _SERVER_HPP_

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

using namespace boost::asio::ip;

class Server {
	public:
        Server(boost::asio::io_service& ioService, int port = 4242);
		~Server();
        void start();
	private:	
        void acceptHandler(const boost::system::error_code& error);
        void printError(const boost::system::error_code& error);

        boost::asio::io_service& _ioService;
        tcp::acceptor _tcpAcceptor;
        std::vector<tcp::socket*> _clients;
};

#endif
