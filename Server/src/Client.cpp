#include <boost/bind.hpp>
#include "Client.hpp"
#include "Server.hpp"

Client::Client(boost::asio::io_service& ioService, Server* server) : 
	_server(server), _tcpSocket(ioService),  _headerRead(false), _tcpBuffer(NULL), _tcpWriteStream(&_tcpSocket) {

}

Client::~Client() {

}

tcp::socket* Client::getSocket() {
	return &_tcpSocket;
}

void Client::startRead() {
	boost::asio::async_read(_tcpSocket, boost::asio::buffer(&_readSize, sizeof(int)), boost::bind(&Client::tcpReadHandler, this, _1, _2));
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
	_server->tcpDataReceived(_readSize, _tcpBuffer);
}

void Client::tcpWrite(int size, void* data) {
	_tcpWriteStream.write(size, data);
}