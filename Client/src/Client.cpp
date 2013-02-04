#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>

#include "Client.hpp"

Client::Client(boost::asio::io_service& ioService)
    : _tcpSocket(ioService), _headerRead(false), _tcpBuffer(NULL)
{
}

Client::~Client() {
    delete [] _tcpBuffer;

    boost::system::error_code ignoredErrorCode;
    _tcpSocket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignoredErrorCode);
    _tcpSocket.close(ignoredErrorCode);
}

void Client::connect(const tcp::endpoint& peerEndpoint) {
    _tcpSocket.async_connect(peerEndpoint,
                             boost::bind(
                                 &Client::connectHandler,
                                 this,
                                 boost::asio::placeholders::error
                                 )
                             );
}

void Client::connectHandler(const boost::system::error_code& error) {
    if (! error) {
        boost::asio::async_read(_tcpSocket,
                                boost::asio::buffer(&_readSize, sizeof(int)), //sizeof(int) differs over plattforms; endianess(!)
                                boost::bind(
                                    &Client::tcpReadHandler, this,
                                    boost::asio::placeholders::error,
                                    boost::asio::placeholders::bytes_transferred
                                    )
                                );
	}
    else {
        printError(error);
    }
}

void Client::tcpReadSizeHandler(const boost::system::error_code& error, std::size_t bytes_transferred) {
    if (! error) {
        _tcpBuffer = new char[_readSize];

        boost::asio::async_read(_tcpSocket,
                                boost::asio::buffer(_tcpBuffer, _readSize),
                                boost::asio::transfer_exactly(_readSize),
                                boost::bind(
                                    &Client::tcpReadHandler,
                                    this,
                                    boost::asio::placeholders::error,
                                    boost::asio::placeholders::bytes_transferred
                                    )
                                );
    } else {
        printError(error);
    }
}

void Client::tcpReadHandler(const boost::system::error_code &error, std::size_t bytes_transferred) {
    if (! error) {
        //do something with your recieved data here
    } else {
        printError(error);
    }
}

void Client::tcpBufferReceived(void) {
	std::cout << _readSize << std::endl;
}

void Client::tcpWriteHandler(const boost::system::error_code& error, std::size_t bytes_transferred) {
    if (! error) {
        // eventually start recieving again or do what ever you want...
    } else {
        printError(error);
    }
}

void Client::tcpWrite(int size, void* data) {

}

void Client::printError(const boost::system::error_code& error) {
    std::cerr << "Client: Error " << error.value() << " " << error.message() << std::endl;
}
