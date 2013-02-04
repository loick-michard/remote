#include <iostream>
#include <boost/asio.hpp>
#include "Client.hpp"

using namespace boost::asio::ip;

int main(int argc, char** argv) {
	if (argc == 3) {
        boost::asio::io_service	ioService;

        Client client(ioService);
        tcp::endpoint endpoint(address::from_string(argv[1]), std::atoi(argv[2]));
        client.connect(endpoint);

		ioService.run();
	}
	else
		std::cerr << "Usage: ./Client adress port" << std::endl;
    return 0;
}
