#include <iostream>

#include <boost/asio/io_service.hpp>

#include "Client.hpp"

using namespace boost::asio::ip;

int main(int argc, char** argv) {
	if (argc == 3) {
        boost::asio::io_service	ioService;

        Client client(ioService);

        try {
            tcp::endpoint endpoint(address::from_string(argv[1]), std::atoi(argv[2]));
            client.connect(endpoint);

            ioService.run();
        } catch (boost::system::system_error& error) {
            std::cerr << "Main: Error " << error.what() << std::endl;
        }
	}
	else
		std::cerr << "Usage: ./Client adress port" << std::endl;
    return 0;
}
