#include "Client.hpp"
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <iostream>

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
		//throw new std::exception("Can't connect");
		std::cout << "Can't connect" << std::endl;
		system("pause");
		exit(-1);
	}
	else {
		std::cout << "Connection success" << std::endl;
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

int loop(const std::string& url);

void Client::tcpBufferReceived(void) {
	std::string str(_tcpBuffer, _readSize);
	std::cout << "Connecting to " << str << std::endl;
	loop(str);
}

void Client::tcpWrite(int size, void* data) {
	_tcpWriteStream.write(size, data);
}