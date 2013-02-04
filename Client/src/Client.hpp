#ifndef _CLIENT_HPP_
# define _CLIENT_HPP_

# include <boost/asio.hpp>
using namespace boost::asio::ip;

class Client {
public:
	Client(boost::asio::io_service&);
	~Client();

	void connect(const tcp::endpoint& peerEndpoint);
	void connectHandler(const boost::system::error_code&);
private:
	tcp::socket	_socket;
};

#endif