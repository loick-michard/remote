#include "Client.hpp"
#include <boost/bind.hpp>

Client::Client(boost::asio::io_service& ioService) : _socket(ioService) {

}

Client::~Client() {

}

void Client::connect(const tcp::endpoint& peerEndpoint) {
	_socket.async_connect(peerEndpoint, boost::bind(&Client::connectHandler, this, _1));
}

void Client::connectHandler(const boost::system::error_code& error) {
	if (error) {
		throw new std::exception("Can't connect");
	}

}