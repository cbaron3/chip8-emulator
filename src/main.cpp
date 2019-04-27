// Main file
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

#include <chrono>
#include <thread>

#include "SDL2/SDL.h"

#include "../include/Interpreter.h"
#include "../include/Chip8.h"
#include "../include/Memory.h"
#include "../include/Logger.h"

// Used for loading the rom
std::unique_ptr<chip8::MemoryMap> load_rom(std::string rom_file_path);

// Constant screen size variables
const unsigned int SDL_SCRN_WIDTH = 1024, SDL_SCRN_HEIGHT = 512;

// SDl2 keys in order for CHIP8
const std::array<unsigned int, 16> key_types = { SDLK_x, SDLK_1, SDLK_2, SDLK_3, SDLK_q, SDLK_w, SDLK_e, SDLK_a, SDLK_s, SDLK_d, SDLK_z, SDLK_c, SDLK_4, SDLK_r, SDLK_f, SDLK_v,};

int main(int argc, char **argv){
	std::string file_path = "";

	// Command line arg input. Lack of command line args were used for debugging
	switch (argc) {
	    case 1:
	    {
				util::Logger::getInstance()->set_max_log_level(LOGTYPE::DEBUG);
	      util::LOG(LOGTYPE::ERROR, "No CL arguments supplied.");
	    } break;
	    case 2:
	    {
	   	  file_path = (argv[1]);
	      util::LOG(LOGTYPE::DEBUG, "ROM: " + file_path + " selected.");
				util::Logger::getInstance()->set_max_log_level(LOGTYPE::ERROR);
	    } break;
	    default:
	    {
				util::Logger::getInstance()->set_max_log_level(LOGTYPE::ERROR);
	      util::LOG(LOGTYPE::ERROR, "Invalid CL arguments supplied.");
	  } break;
	}

	util::LOG(LOGTYPE::DEBUG, "ROM selected, starting program...");

	// Initialize memory map
	std::unique_ptr<chip8::MemoryMap> memory_map = std::move(load_rom(file_path));

	// Initialize interpreter
	std::unique_ptr<chip8::Interpreter> interpreter = chip8::Interpreter::make_interpreter(std::move(memory_map));

	// Setup SDL2
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

	// SDL Rendereder
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_RenderSetLogicalSize(renderer, SDL_SCRN_WIDTH, SDL_SCRN_HEIGHT);

	// Create a texture. want ARGB 8888 renderer meaning uint32_t elements
	SDL_Texture* texture = SDL_CreateTexture( renderer, 
											  SDL_PIXELFORMAT_ARGB8888,
											  SDL_TEXTUREACCESS_STREAMING,
											  64,
											  32);

	// Game loop
	std::array<bool, 16> key_state = {};	
	for(;;)
	{
		interpreter->next_instruction();

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

		// Update key map. SDL key events modify global key state array. Dont make it global
		interpreter->sync_keys(key_state);

		// Draw instruction
		if( interpreter->draw() == true )
		{
			// SDL 2.0	
			util::LOG(LOGTYPE::DEBUG, "Draw flag set, prepping screen state for texture update");

			SDL_UpdateTexture(texture, NULL, interpreter->screen().data(), 64*sizeof(uint32_t));
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

	while(mem_adr < chip8::PROG_START)
	{
		memory_map->store( (std::byte) 0, mem_adr++);
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

	return memory_map;
}
