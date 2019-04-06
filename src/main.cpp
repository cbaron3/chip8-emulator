// Main file
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <chrono>
#include <thread>

#include "SDL2/SDL.h"

#include "../include/Interpreter.h"
#include "../include/Chip8.h"
#include "../include/Memory.h"
#include "../include/Logger.h"


const std::array<unsigned int, 16> key_types = {
	SDLK_1,		// 1
	SDLK_2,		// 2	
	SDLK_3,		// 3
	SDLK_4,		// C
	SDLK_q,		// 4
	SDLK_w,		// 5
	SDLK_e,		// 6
	SDLK_r,		// D
	SDLK_a,		// 7
	SDLK_s,		// 8
	SDLK_d,		// 9
	SDLK_f,		// E
	SDLK_z,		// A
	SDLK_x,		// 0
	SDLK_c,		// B
	SDLK_v		// F
};

const unsigned int SDL_SCRN_WIDTH = 1024;
const unsigned int SDL_SCRN_HEIGHT = 512;
 
std::array<bool, 16> key_state = {};	

std::unique_ptr<chip8::MemoryMap> load_rom(std::string rom_file_path);

int main(int argc, char **argv){
	std::string file_path = "../roms/programs/Chip8 emulator Logo [Garstyciuks].ch8";

	// Skeleton for input parsing
	switch (argc) {
	    case 1:
	    {
	      std::cout << "No CL arguments supplied" << std::endl;
	      break;
	    }
	    case 2:
	    {
	   	  std::string s(argv[1]);
	      util::LOG(LOGTYPE::DEBUG, s);
	      break;
	    }
	    default:
	    {
	      std::cout << "Invalid CL arguments supplied" << std::endl;
	      break;
	  }
	}

	util::Logger::getInstance()->set_max_log_level(LOGTYPE::DEBUG);

	util::LOG(LOGTYPE::DEBUG, "ROM selected, starting program...");

	// Initialize memory map
	std::unique_ptr<chip8::MemoryMap> memory_map = std::move(load_rom(file_path));

	// Load ROM
	std::cout << (*memory_map);

	// Initialize interpreter
	std::unique_ptr<chip8::Interpreter> interpreter = chip8::Interpreter::make_interpreter(std::move(memory_map));

	// Setup SDL2
	/* TODO */
	SDL_Window* window = NULL;

	// Initialize SDL
	if( SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::string err = std::string("SDL could not Initialize! SDL_Error: ") + SDL_GetError();
		util::LOG(LOGTYPE::ERROR, err);
		exit(1);
	}

	// Create the SDL2 window
	window = SDL_CreateWindow( "Chip8 Interpreter",
								SDL_WINDOWPOS_UNDEFINED,
								SDL_WINDOWPOS_UNDEFINED,
								SDL_SCRN_WIDTH,
								SDL_SCRN_HEIGHT,
								SDL_WINDOW_SHOWN );

	if( window == NULL )
	{
		std::string err = std::string("Window could not be created! SDL_Error: ") + SDL_GetError();
		util::LOG(LOGTYPE::ERROR, err);
		exit(2);
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_RenderSetLogicalSize(renderer, SDL_SCRN_WIDTH, SDL_SCRN_HEIGHT);

	// Create a texture. want ARGB 8888 renderer meaning uint32_t elements
	SDL_Texture* texture = SDL_CreateTexture( renderer, 
											  SDL_PIXELFORMAT_ARGB8888,
											  SDL_TEXTUREACCESS_STREAMING,
											  64,
											  32);


	uint32_t pixels[2048];

	// Game loop
	for(;;)
	{
		interpreter->next_instruction();

		SDL_Event e;
		while(SDL_PollEvent(&e))
		{
			if( e.type == SDL_QUIT)
			{
				exit(0);
			}

			if(e.type == SDL_KEYDOWN)
			{
				if(e.key.keysym.sym == SDLK_ESCAPE)
				{
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

			if(e.type == SDL_KEYUP)
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
		// Update key map. SDL key events modify global key state array. Dont make it global
		interpreter->sync_keys(key_state);

		if( interpreter->draw() == true )
		{
			// SDL 2.0	
			util::LOG(LOGTYPE::DEBUG, "Draw flag set, prepping screen state for texture update");
			std::array<std::array<bool, 64>, 32> screen_state = interpreter->screen();


			unsigned int x = 0;
			for( int i = 0; i < 32; ++i )
			{
				for( int j = 0; j < 64; ++j )
				{
					if(screen_state[i][j] == true)
					{
						pixels[x++] = 0xFFFFFFFF;
					}
					else
					{
						pixels[x++] = 0x00000000;
					}
					
					
				}
			}

			util::LOG(LOGTYPE::DEBUG, "Draw flag set, screen prepped for update");

			SDL_UpdateTexture(texture, NULL, pixels, 64*sizeof(uint32_t));
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			SDL_RenderPresent(renderer);	
		}

		std::this_thread::sleep_for(std::chrono::microseconds(1200));
	}

	util::LOG(LOGTYPE::DEBUG, "ROM finished executing, ending program...");
	return 0;
}

std::unique_ptr<chip8::MemoryMap> load_rom(std::string rom_file_path)
{	
	std::unique_ptr<chip8::MemoryMap> memory_map = chip8::MemoryMap::makeMemoryMap(4096);

	// Load rom into memory map starting at 0
	unsigned int mem_adr = chip8::FONT_START;

	// Chip8 environment variables are static constants in Chip8Interpreter
	for(auto &font_byte : chip8::FONTSET)
	{
		memory_map->store( (std::byte) font_byte, mem_adr++);
	}

	// Open rom file
	std::ifstream f_rom( rom_file_path );
	mem_adr = chip8::PROG_START;
	
	// Store every byte in the rom into the memory map
	if( f_rom.is_open() )
	{
		unsigned char rom_byte = f_rom.get();

		while( f_rom.good() )
		{
			memory_map->store( (std::byte) rom_byte, mem_adr++);
			rom_byte = f_rom.get();
		}
	}
	else
	{
		util::LOG(LOGTYPE::ERROR, "File: " + rom_file_path + " failed to open.");
	}

	// Store rest of prog space with 0s cause apparently undefined memory locs shuold be 0
	while(mem_adr < 4096)
	{
		memory_map->store( (std::byte) 0, mem_adr++);
	}

	return memory_map;
}
