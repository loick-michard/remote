#ifndef _CLIENT_HPP_
# define _CLIENT_HPP_

# include <boost/asio.hpp>
# include <queue>

using namespace boost::asio::ip;

class Client {
public:
    Client(boost::asio::io_service& ioService);
	~Client();

    void connect(const tcp::endpoint& peerEndpoint);
private:
    void connectHandler(const boost::system::error_code& error);
    void tcpReadSizeHandler(const boost::system::error_code& error, std::size_t bytes_transferred);
    void tcpReadHandler(const boost::system::error_code& error, std::size_t bytes_transferred);
    void tcpBufferReceived(void);
    void tcpWriteHandler(const boost::system::error_code& error, std::size_t bytes_transferred);
    void tcpWrite(int size = -1, void* data = NULL);
    void printError(const boost::system::error_code& error);

	tcp::socket	_tcpSocket;
	int			_readSize;
	bool		_headerRead;
	char*		_tcpBuffer;
};

#endif
