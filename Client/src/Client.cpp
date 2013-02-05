#include "Client.hpp"
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>

Client::Client(boost::asio::io_service& ioService) : 
	_tcpSocket(ioService), _headerRead(false), _tcpBuffer(NULL), _tcpWriteStream(&_tcpSocket) {

}

Client::~Client() {

}

void Client::connect(const tcp::endpoint& peerEndpoint) {
	_tcpSocket.async_connect(peerEndpoint, boost::bind(&Client::connectHandler, this, _1));
}

void Client::connectHandler(const boost::system::error_code& error) {
	if (error) {
		throw new std::exception("Can't connect");
	}
	else {
		std::cout << "Connection success" << std::endl;
		int size = 5;
		tcpWrite(sizeof(int), &size);
		tcpWrite(5, "salut");
		boost::asio::async_read(_tcpSocket, boost::asio::buffer(&_readSize, sizeof(int)), boost::bind(&Client::tcpReadHandler, this, _1, _2));
	}
}

void Client::tcpReadHandler(const boost::system::error_code& error, std::size_t bytes_transferred) {
	if (error) {
		exit(0);
	}
	if (!_headerRead) {
		if (_tcpBuffer)
			delete [] _tcpBuffer;
		_tcpBuffer = new char[_readSize];
		_headerRead = true;
		boost::asio::async_read(_tcpSocket, boost::asio::buffer(_tcpBuffer, _readSize), boost::bind(&Client::tcpReadHandler, this, _1, _2));
	}
	else {
		_headerRead = false;
		tcpBufferReceived();
		boost::asio::async_read(_tcpSocket, boost::asio::buffer(&_readSize, sizeof(int)), boost::bind(&Client::tcpReadHandler, this, _1, _2));
	}
}

void Client::tcpBufferReceived(void) {
	std::cout << _readSize << std::endl;
}

void Client::tcpWrite(int size, void* data) {
	_tcpWriteStream.write(size, data);
}