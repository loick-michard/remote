#include <iostream>
#include <boost/asio.hpp>
#include "Client.hpp"

using namespace boost::asio::ip;

int main(int argc, char** argv) {
	try {
	if (argc == 3) {
		boost::asio::io_service			ioService;
		tcp::resolver					resolver(ioService);

		Client client(ioService);
		tcp::endpoint endpoint(address::from_string(argv[1]), std::atoi(argv[2]));
		client.connect(endpoint);

		ioService.run();
	}
	else
		std::cerr << "Usage: ./Client adress port" << std::endl;
	}
	catch (std::exception* e) {
		std::cerr << e->what() << std::endl;
		delete e;
	}
	return (0);
}