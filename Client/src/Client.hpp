#ifndef _CLIENT_HPP_
# define _CLIENT_HPP_

# include <boost/asio.hpp>
# include <queue>
# include "WriteStream.hpp"

using namespace boost::asio::ip;

class Client {
public:
	Client(boost::asio::io_service&);
	~Client();

	void connect(const tcp::endpoint& peerEndpoint);
	void connectHandler(const boost::system::error_code&);
	void tcpReadHandler(const boost::system::error_code& error, std::size_t bytes_transferred);
	void tcpBufferReceived(void);
	void tcpWrite(int size = -1, void* data = NULL);
private:
	tcp::socket	_tcpSocket;
	int			_readSize;
	bool		_headerRead;
	char*		_tcpBuffer;
	WriteStream<tcp::socket>	_tcpWriteStream;
};

#endif