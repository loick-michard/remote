#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <string>

#include <SDL.h>
#undef main
#include <SDL_mutex.h>

#include <vlc/vlc.h>

#include <boost/asio.hpp>
#include "Client.hpp"

#define WIDTH 1280
#define HEIGHT 720

#define VIDEOWIDTH 1280
#define VIDEOHEIGHT 720

struct ctx
{
    SDL_Surface *surf;
    SDL_mutex *mutex;
};

static void *lock(void *data, void **p_pixels)
{
    struct ctx *ctx = (struct ctx*)data;

    SDL_LockMutex(ctx->mutex);
    SDL_LockSurface(ctx->surf);
    *p_pixels = ctx->surf->pixels;
    return NULL; /* picture identifier, not needed here */
}

static void unlock(void *data, void *id, void *const *p_pixels)
{
    struct ctx *ctx = (struct ctx*)data;

    /* VLC just rendered the video, but we can also render stuff */
    uint16_t *pixels = *((uint16_t**)p_pixels);
    int x, y;

    SDL_UnlockSurface(ctx->surf);
    SDL_UnlockMutex(ctx->mutex);

    assert(id == NULL); /* picture identifier, not needed here */
}

static void display(void *data, void *id)
{
    /* VLC wants to display the video */
    (void) data;
    assert(id == NULL);
}

int loop(const std::string& url) {
	   struct ctx ctx;

	libvlc_instance_t *libvlc;
    libvlc_media_t *m;
    libvlc_media_player_t *mp;
    char const *vlc_argv[] =
    {
        "--no-audio", /* skip any audio track */
        "--no-xlib", /* tell VLC to not use Xlib */
    };
    int vlc_argc = sizeof(vlc_argv) / sizeof(*vlc_argv);

    SDL_Surface *screen, *empty;
    SDL_Event event;
    SDL_Rect rect;
    int done = 0, action = 0, pause = 0, n = 0;

    /*
     *  Initialise libSDL
     */
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        printf("cannot initialize SDL\n");
        return EXIT_FAILURE;
    }

    empty = SDL_CreateRGBSurface(SDL_SWSURFACE, VIDEOWIDTH, VIDEOHEIGHT,
                                 32, 0, 0, 0, 0);
    ctx.surf = SDL_CreateRGBSurface(SDL_SWSURFACE, VIDEOWIDTH, VIDEOHEIGHT,
                                    16, 0x001f, 0x07e0, 0xf800, 0);

    ctx.mutex = SDL_CreateMutex();

    int options = SDL_ANYFORMAT | SDL_HWSURFACE | SDL_DOUBLEBUF;

	SDL_WM_SetCaption("Remote", NULL);
    screen = SDL_SetVideoMode(WIDTH, HEIGHT, 0, options);
    if(!screen)
    {
        printf("cannot set video mode\n");
        return EXIT_FAILURE;
    }

    /*
     *  Initialise libVLC
     */
    libvlc = libvlc_new(vlc_argc, vlc_argv);
	m = libvlc_media_new_location(libvlc, url.c_str());
    mp = libvlc_media_player_new_from_media(m);
    libvlc_media_release(m);

    libvlc_video_set_callbacks(mp, lock, unlock, display, &ctx);
    libvlc_video_set_format(mp, "RV16", VIDEOWIDTH, VIDEOHEIGHT, VIDEOWIDTH*2);
    libvlc_media_player_play(mp);

    /*
     *  Main loop
     */
    rect.w = 0;
    rect.h = 0;

    while(!done)
    { 
        action = 0;

        /* Keys: enter (fullscreen), space (pause), escape (quit) */
        while( SDL_PollEvent( &event ) ) 
        { 
            switch(event.type)
            {
            case SDL_QUIT:
                done = 1;
                break;
            case SDL_KEYDOWN:
                action = event.key.keysym.sym;
                break;
            }
        }

        switch(action)
        {
        case SDLK_ESCAPE:
            done = 1;
            break;
        case SDLK_RETURN:
            options ^= SDL_FULLSCREEN;
            screen = SDL_SetVideoMode(WIDTH, HEIGHT, 0, options);
            break;
        case ' ':
            pause = !pause;
            break;
        }

        rect.x = (int)((1. + .5 * sin(0.03 * n)) * (WIDTH - VIDEOWIDTH) / 2);
        rect.y = (int)((1. + .5 * cos(0.03 * n)) * (HEIGHT - VIDEOHEIGHT) / 2);

        if(!pause)
            n++;

        /* Blitting the surface does not prevent it from being locked and
         * written to by another thread, so we use this additional mutex. */
        SDL_LockMutex(ctx.mutex);
        SDL_BlitSurface(ctx.surf, NULL, screen, &rect);
        SDL_UnlockMutex(ctx.mutex);

        SDL_Flip(screen);
        SDL_Delay(10);

        SDL_BlitSurface(empty, NULL, screen, &rect);
    }

    /*
     * Stop stream and clean up libVLC
     */
    libvlc_media_player_stop(mp);
    libvlc_media_player_release(mp);
    libvlc_release(libvlc);

    /*
     * Close window and clean up libSDL
     */
    SDL_DestroyMutex(ctx.mutex);
    SDL_FreeSurface(ctx.surf);
    SDL_FreeSurface(empty);

    SDL_Quit();

	return (0);
}

int main(int argc, char *argv[])
{

 
    std::cout << "Enter server ip address:" << std::endl;
	std::string ip;
	std::cin >> ip;


	boost::asio::io_service service;

	Client client(service);
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(ip), 4242);
	client.connect(endpoint);

	service.run();

    return 0;
}