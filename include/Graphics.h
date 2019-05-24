#ifndef GRAPHICS_SDL2_HPP
#define GRAPHICS_SDL2_HPP

#include <string>

#include "SDL2/SDL.h"
#include "Singleton.h"
#include "Logger.h"

/*!
 *  \addtogroup chip8
 *  @{
 */

//! chip8 code
namespace chip8
{

class Graphics : public Singleton<Graphics>
{

    friend class Singleton<Graphics>;

public:
    Graphics( void )
    {
        p_window = NULL;
        p_renderer = NULL;
        p_texture = NULL;

        key_state = {};
    }

    void init( )
    {
        // Initialize SDL
        if( SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            std::string err = std::string("SDL could not Initialize! SDL_Error: ") + SDL_GetError();
            util::LOG(LOGTYPE::ERROR, err);
            exit(1);
        }

        init_window();

        init_texture();
    }

    // For key strokes
    std::array<bool, 16> check_events()
    {
        SDL_Event e;
		while(SDL_PollEvent(&e))
		{
			if(e.type == SDL_KEYDOWN)
			{
				if(e.key.keysym.sym == SDLK_ESCAPE)
				{
					util::LOG(LOGTYPE::ERROR, "Exiting program");
					exit(0);
				}

				for (int i = 0; i < 16; ++i)
				{
					if (e.key.keysym.sym == key_types[i])
					{
						key_state[i] = true;
					}
				}
			}
			else if(e.type == SDL_KEYUP)
			{
				for (int i = 0; i < 16; ++i)
				{
					if (e.key.keysym.sym == key_types[i])
					{
						key_state[i] = false;
					}
				}
			}
		}

        return key_state;
    }

    // Update texture
    void update_texture( std::array<uint32_t, 64 * 32> screen )
    {
        // SDL 2.0	
        util::LOG(LOGTYPE::DEBUG, "Draw flag set, prepping screen state for texture update");

        SDL_UpdateTexture(p_texture, NULL, screen.data(), 64*sizeof(uint32_t));
        SDL_RenderClear(p_renderer);
        SDL_RenderCopy(p_renderer, p_texture, NULL, NULL);
        SDL_RenderPresent(p_renderer);	
    }

protected:
private:
    static const unsigned int SDL_SCRN_WIDTH = 1024, SDL_SCRN_HEIGHT = 512;

    void init_window()
    {
        // Create the SDL2 window
        p_window = SDL_CreateWindow( "Chip8 Interpreter",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_SCRN_WIDTH,
                                    SDL_SCRN_HEIGHT,
                                    SDL_WINDOW_SHOWN );

        if( p_window == NULL )
        {
            std::string err = std::string("Window could not be created! SDL_Error: ") + SDL_GetError();
            util::LOG(LOGTYPE::ERROR, err);
            exit(2);
        }
    }

    void init_texture()
    {
        // SDL Rendereder
        p_renderer = SDL_CreateRenderer(p_window, -1, 0);
        SDL_RenderSetLogicalSize(p_renderer, SDL_SCRN_WIDTH, SDL_SCRN_HEIGHT);

        // Create a texture. want ARGB 8888 renderer meaning uint32_t elements
        p_texture = SDL_CreateTexture( p_renderer, 
                                                SDL_PIXELFORMAT_ARGB8888,
                                                SDL_TEXTUREACCESS_STREAMING,
                                                64,
                                                32);
    }

    SDL_Window*     p_window;
    SDL_Renderer*   p_renderer;
    SDL_Texture*    p_texture;

    const std::array<unsigned int, 16> key_types = { SDLK_x, SDLK_1, SDLK_2, SDLK_3, 
                                                    SDLK_q, SDLK_w, SDLK_e, SDLK_a, 
                                                    SDLK_s, SDLK_d, SDLK_z, SDLK_c, 
                                                    SDLK_4, SDLK_r, SDLK_f, SDLK_v,};

    std::array<bool, 16> key_state;
};

} // namespace chip8

/*! @} End of Doxygen Groups*/


#endif 