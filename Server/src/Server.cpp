#include <boost/bind.hpp>
#include "Server.hpp"
#include "Client.hpp"

Server::Server(boost::asio::io_service& ioService, int port) : _tcpAcceptor(ioService, tcp::endpoint(tcp::v4(), port)) {
	_ioService = &ioService;
	_clients.push_back(new Client(ioService, this));
	_tcpAcceptor.async_accept(*(_clients.back()->getSocket()), boost::bind(&Server::acceptHandler, this, _1));
	std::cout << "Waiting connection on port 4242" << std::endl;
}

Server::~Server() {
	for (unsigned int i = 0; i < _clients.size(); ++i)
		delete _clients[i];
}


void Server::acceptHandler(const boost::system::error_code& error) {
	if (!error) {
		std::cout << "New connection from " << _clients.back()->getSocket()->remote_endpoint().address().to_string() << std::endl;
		_clients.back()->startRead();
		_clients.push_back(new Client(*_ioService, this));
		_tcpAcceptor.async_accept(*_clients.back()->getSocket(), boost::bind(&Server::acceptHandler, this, _1));
	}
}

void Server::tcpWriteToAll(int size, void* data) {
	for (unsigned int i = 0; i < _clients.size(); ++i)
		_clients[i]->tcpWrite(size, data);
}

void Server::tcpDataReceived(int size, char* data) {
	std::cout << size << " received from clients" << std::endl;
}