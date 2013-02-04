#include <boost/bind.hpp>
#include "Server.hpp"

Server::Server(boost::asio::io_service& ioService, int port) : _tcpAcceptor(ioService, tcp::endpoint(tcp::v4(), port)) {
	_ioService = &ioService;
	_clients.push_back(new tcp::socket(*_ioService));
	_tcpAcceptor.async_accept(*(_clients.back()), boost::bind(&Server::acceptHandler, this, _1));
	std::cout << "Waiting connection on port 4242" << std::endl;
}

Server::~Server() {
	for (int i = 0; i < _clients.size(); ++i)
		delete _clients[i];
}


void Server::acceptHandler(const boost::system::error_code& error) {
	if (!error) {
		std::cout << "New connection from " << _clients.back()->remote_endpoint().address().to_string() << std::endl;
		_clients.push_back(new tcp::socket(*_ioService));
		_tcpAcceptor.async_accept(*_clients.back(), boost::bind(&Server::acceptHandler, this, _1));
	}
}