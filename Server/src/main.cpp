#include <iostream>
#include <boost/asio.hpp>
#include "Server.hpp"

using namespace boost::asio::ip;

int main(int argc, char** argv) {
	try {
		boost::asio::io_service			ioService;
		Server server(ioService, (argc == 2) ? std::atoi(argv[1]) : 4242);
		ioService.run();
	}
	catch (std::exception* e) {
		std::cerr << e->what() << std::endl;
		delete e;
	}
	return (0);
}