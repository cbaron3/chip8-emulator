// Main file
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <chrono>
#include <thread>

// #include <SDL2/SDL>

#include "../include/Interpreter.h"
#include "../include/Chip8.h"
#include "../include/Memory.h"
#include "../include/Logger.h"


// const std::array<uint8_t, 16> controller_inputs_ = {
// 	SDLK_1,		// 1
// 	SDLK_2,		// 2	
// 	SDLK_3,		// 3
// 	SDLK_4,		// C
// 	SDLK_q,		// 4
// 	SDLK_w,		// 5
// 	SDLK_e,		// 6
// 	SDLK_r,		// D
// 	SDLK_a,		// 7
// 	SDLK_s,		// 8
// 	SDLK_d,		// 9
// 	SDLK_f,		// E
// 	SDLK_z,		// A
// 	SDLK_x,		// 0
// 	SDLK_c,		// B
// 	SDLK_v		// F
// }
 
std::array<bool, 16> key_state = {};	

std::unique_ptr<chip8::MemoryMap> load_rom(std::string rom_file_path);

int main(int argc, char **argv){
	std::string file_path = "../roms/full_games/PONG";

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

	// Game loop
	for(;;)
	{
		interpreter->next_instruction();

		// Update key map. SDL key events modify global key state array. Dont make it global
		interpreter->sync_keys(key_state);

		if( interpreter->draw() == true )
		{
			// SDL 2.0	
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));		
		}

		// std::this_thread::sleep_for(std::chrono::microseconds(1200));
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

	return memory_map;
}
