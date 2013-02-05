#include <iostream>
#include <boost/asio.hpp>
#include <vlc/vlc.h>
#include <SFML/Graphics.hpp>
#include "Client.hpp"

using namespace boost::asio::ip;

unsigned char* frame = NULL;
unsigned char* frame2 = NULL;
sf::Mutex mutex;

void* vlc_callback_lock(void* param, void** planes) {
	//std::cout << "FRAME!\n";
	mutex.lock();
	*planes = frame;
	return NULL;
}

void vlc_callback_unlock(void* param, void* picture, void* const* planes) {
	mutex.unlock();
	return ;
}

void vlc_callback_display(void* param, void* picture) {
	return ;
}


int main(int argc, char** argv) {
	try {

		sf::RenderWindow window(sf::VideoMode(640, 480), "SFML works!");

		libvlc_instance_t* vlc;
		libvlc_media_player_t* player;
		libvlc_media_t* media;

		vlc = libvlc_new(0, NULL);

		if (vlc == NULL) {
			std::cerr << "Lib vlc initialization failure" << std::endl;
			Sleep(5000);
			return (-1);
		}

		media = libvlc_media_new_location(vlc, "udp://@:1234");

		if (media == NULL) {
			std::cerr << "Cannot open media" << std::endl;
			Sleep(5000);
			return (-1);
		}

		player = libvlc_media_player_new_from_media(media);

		libvlc_media_release(media);
		
		frame = new unsigned char[640 * 480 * 4];
		frame2 = new unsigned char[640 * 480 * 4];

		libvlc_video_set_callbacks(player, vlc_callback_lock, vlc_callback_unlock, vlc_callback_display, NULL);
		libvlc_video_set_format(player, "RV32", 640, 480, 640*4);
		libvlc_media_player_play(player);

		sf::Texture texture;
		texture.create(640, 480);
		texture.update(frame);
		sf::Sprite sprite(texture);

		while (window.isOpen()) {
			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					window.close();
			}

			window.clear();

			mutex.lock();

			for (int i = 0; i < 640 * 480; ++i) {
				frame2[i * 4 + 0] = frame[i * 4 + 2];
				frame2[i * 4 + 1] = frame[i * 4 + 1];
				frame2[i * 4 + 2] = frame[i * 4 + 0];
				frame2[i * 4 + 3] = frame[i * 4 + 3];
			}

			texture.update(frame2);
			mutex.unlock();

			window.draw(sprite);
			window.display();
		}

		return (0);

		// Boost stuff
	if (argc == 3) {
		boost::asio::io_service			ioService;

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