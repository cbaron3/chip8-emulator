// Main file
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "../include/CPU.h"

#include "../include/Emulator.h"
#include "../include/Memory.h"
#include "../include/Logger.h"



void load_rom(std::unique_ptr<MemoryMap> memory_map, std::string rom_file_path)
{
	// Load rom into memory map starting at 0
	unsigned int mem_adr = chip8::Interpreter::FONT_START;

	// Chip8 environment variables are static constants in Chip8Interpreter
	for(uint8_t &font_byte : chip8::Interpreter::font_set)
	{
		memory_map->store( (std::byte) font_byte, mem_adr++);
	}

	// Open rom file
	std::ifstream f_rom( fp );
	mem_adr = chip8::Interpreter::PROG_START;
	
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
		util::LOG(LOGTYPE::ERROR, "File: " + fp + " failed to open.");
	}
}




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
	std::unique_ptr<MemoryMap> memory_map = chip8::MemoryMap::makeMemoryMap(chip8::Interpreter::MEM_SPACE);

	// Load ROM
	load_rom(std::move(memory_map), file_path);
	std::cout << (*memory_map);

	// Initialize interpreter
	std::unique_ptr<Interpreter> interpreter = chip8::Interpreter::make_interpreter(std::move(memory_map));

	// Setup SDL2
	/* TODO */

	// Game loop
	for(;;)
	{
		interpreter->next_instruction();

		if( interpreter.draw() == true )
		{
			// Draw
		}

		interpreter.key_input(/* SDL KEYS */);
	}

	util::LOG(LOGTYPE::DEBUG, "ROM finished executing, ending program...");
	return 0;
}