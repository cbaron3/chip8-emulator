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

/**
 * @brief Singleton graphics class used simply to hide the bulk of SDL code
 */
class Graphics : public Singleton<Graphics>
{

public:
    /**
     * @brief Construct a new Graphics object
     */
    Graphics( void )
    {
        // Initialize members to safe state
        p_window = NULL;
        p_renderer = NULL;
        p_texture = NULL;
        key_state = {};
    }

    /**
     * @brief Initialize graphics window, renderer, and texture
     */
    void init( void )
    {
        // Initialize SDL
        if( SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            std::string err = std::string("SDL could not Initialize! SDL_Error: ") + SDL_GetError();
            util::LOG(LOGTYPE::ERROR, err);
            exit(1);
        }

        // Initialize window
        init_window();
        // Initialize renderer and texture
        init_texture();
    }

    /**
     * @brief Monitor key events for chip8 controller inputs
     * 
     * @return std::array<bool, 16> state of all chip8 inputs. True means pressed, else false.
     */
    std::array<bool, 16> check_events()
    {
        SDL_Event e;
		while(SDL_PollEvent(&e))
		{
            // On keydown, set key state if is chip8 controller input
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
            // On keyup, clear key state if is chip8 controller input
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

        // Return key state
        return key_state;
    }

    /**
     * @brief Update sdl texture and render on screen
     * 
     * @param screen array of pixels defining chip8 screen state of 64x32
     */
    void update_texture( std::array<uint32_t, 64 * 32> screen )
    {
        util::LOG(LOGTYPE::DEBUG, "Draw flag set, prepping screen state for texture update");
        // Magic numbers to fix
        SDL_UpdateTexture(p_texture, NULL, screen.data(), 64*sizeof(uint32_t));
        SDL_RenderClear(p_renderer);
        SDL_RenderCopy(p_renderer, p_texture, NULL, NULL);
        SDL_RenderPresent(p_renderer);	
    }

protected:
private:
    // SDL screen size
    static const unsigned int SDL_SCRN_WIDTH = 1024, SDL_SCRN_HEIGHT = 512;
    // Chip8 controller keys 
    const std::array<unsigned int, 16> key_types = { SDLK_x, SDLK_1, SDLK_2, SDLK_3, 
                                                    SDLK_q, SDLK_w, SDLK_e, SDLK_a, 
                                                    SDLK_s, SDLK_d, SDLK_z, SDLK_c, 
                                                    SDLK_4, SDLK_r, SDLK_f, SDLK_v,};
    // Chip8 controller key states
    std::array<bool, 16> key_state;

    // SDL variables
    SDL_Window*     p_window;
    SDL_Renderer*   p_renderer;
    SDL_Texture*    p_texture;

    // Helper function for window init
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

    // Helper function for texture init
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
};

} // namespace chip8

/*! @} End of Doxygen Groups*/


#endif 