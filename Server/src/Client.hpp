#ifndef _CLIENT_HPP_
# define _CLIENT_HPP_

# include <boost/asio.hpp>
# include "WriteStream.hpp"

using namespace boost::asio::ip;

class Server;

class Client {
	public:
		Client(boost::asio::io_service&, Server*);
		~Client();

		tcp::socket*	getSocket();
		void			startRead();
		void			tcpReadHandler(const boost::system::error_code& error, std::size_t bytes_transferred);
		void			tcpBufferReceived(void);
		void			tcpWrite(int size, void* data);
	private:	
		Server*						_server;
		tcp::socket					_tcpSocket;
		int							_readSize;
		bool						_headerRead;
		char*						_tcpBuffer;
		WriteStream<tcp::socket>	_tcpWriteStream;
};

#endif