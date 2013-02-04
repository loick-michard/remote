#include <boost/bind.hpp>
#include "Server.hpp"

/** A seperate clientConnection class would be better **/
/** This class is not thread-safe!! **/

Server::Server(boost::asio::io_service& ioService, int port)
    : _ioService(ioService), _tcpAcceptor(ioService, tcp::endpoint(tcp::v4(), port))
{
}

Server::~Server() {
    boost::system::error_code ignoredErrorCode;
    _tcpAcceptor.close(ignoredErrorCode);

    for (std::vector<tcp::socket*>::iterator clientIt = _clients.begin(); clientIt != _clients.end(); clientIt++) {
        (*clientIt)->shutdown(tcp::socket::shutdown_both, ignoredErrorCode);
        (*clientIt)->close(ignoredErrorCode);

        delete *clientIt;
    }
}

void Server::start() {
    _clients.push_back(new tcp::socket(_ioService));

    _tcpAcceptor.async_accept(
                *(_clients.back()),
                boost::bind(
                    &Server::acceptHandler,
                    this,
                    boost::asio::placeholders::error
                    )
                );

    std::cout << "Waiting for connection on port 4242" << std::endl;
}

void Server::acceptHandler(const boost::system::error_code& error) {
    if (! error) {
		std::cout << "New connection from " << _clients.back()->remote_endpoint().address().to_string() << std::endl;

        start();
    } else {
        printError(error);
    }
}

void Server::printError(const boost::system::error_code &error) {
    std::cerr << "Server: Error " << error.value() << " " << error.message() << std::endl;
}
