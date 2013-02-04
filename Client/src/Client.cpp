#include <boost/bind.hpp>

#include "Client.hpp"

Client::Client(boost::asio::io_service& ioService)
    : _tcpSocket(ioService)
{
}

Client::~Client() {
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
        uint32_t contentSize;
        boost::asio::async_read(_tcpSocket,
                                boost::asio::buffer(&contentSize, sizeof(uint32_t)), //sizeof(int) differs on plattforms; endianess(!)
                                boost::asio::transfer_exactly(sizeof(uint32_t)),
                                boost::bind(
                                    &Client::tcpReadSizeHandler, this,
                                    boost::asio::placeholders::error,
                                    contentSize
                                    )
                                );
	}
    else {
        printError(error);
    }
}

void Client::tcpReadSizeHandler(const boost::system::error_code& error, uint32_t contentSize) {
    if (! error) {
        boost::shared_array<char> tcpBuffer(new char[contentSize]);

        boost::asio::async_read(_tcpSocket,
                                boost::asio::buffer(tcpBuffer.get(), contentSize),
                                boost::asio::transfer_exactly(contentSize),
                                boost::bind(
                                    &Client::tcpReadHandler,
                                    this,
                                    boost::asio::placeholders::error,
                                    boost::asio::placeholders::bytes_transferred,
                                    tcpBuffer
                                    )
                                );
    } else {
        printError(error);
    }
}

void Client::tcpReadHandler(const boost::system::error_code &error, std::size_t bytesTransferred, boost::shared_array<char> buffer) {
    if (! error) {
        //do something with your recieved data here
    } else {
        printError(error);
    }
}

void Client::tcpWriteHandler(const boost::system::error_code& error, std::size_t bytesTransferred) {
    if (! error) {
        // eventually start recieving again or do what ever you want...
    } else {
        printError(error);
    }
}

void Client::tcpWrite(boost::shared_array<char> data, std::size_t size) {

}

void Client::printError(const boost::system::error_code& error) {
    std::cerr << "Client: Error " << error.value() << " " << error.message() << std::endl;
}
