#ifndef _CLIENT_HPP_
#define _CLIENT_HPP_

#include <boost/shared_array.hpp>
#include <boost/asio.hpp>

using namespace boost::asio::ip;

class Client {
public:
    Client(boost::asio::io_service& ioService);
	~Client();

    void connect(const tcp::endpoint& peerEndpoint);
private:
    void connectHandler(const boost::system::error_code& error);
    void tcpReadSizeHandler(const boost::system::error_code& error, uint32_t contentSize);
    void tcpReadHandler(const boost::system::error_code& error, std::size_t bytes_transferred, boost::shared_array<char> buffer);
    void tcpWriteHandler(const boost::system::error_code& error, std::size_t bytes_transferred);
    void tcpWrite(boost::shared_array<char> data, std::size_t size);
    void printError(const boost::system::error_code& error);

	tcp::socket	_tcpSocket;
};

#endif
