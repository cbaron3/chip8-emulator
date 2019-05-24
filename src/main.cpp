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
#include "../include/Memory.h"
#include "../include/Graphics.h"
#include "../include/Logger.h"

// Used for loading the rom
std::unique_ptr<chip8::MemoryMap> load_rom(std::string rom_file_path);

int main(int argc, char **argv){
	std::string file_path = "";

	// Process input arguments. No checks right now for proper file
	if( argc == 2 )
	{
		file_path = (argv[1]);
		util::LOG(LOGTYPE::DEBUG, "ROM: " + file_path + " selected.");
		util::Logger::get_instance()->set_max_log_level(LOGTYPE::ERROR);
	}
	else
	{
		util::LOG(LOGTYPE::ERROR, "Invalid CL arguments supplied. Quitting.");
		exit(1);
	}

	// Initialize memory map
	std::unique_ptr<chip8::MemoryMap> memory_map = std::move(load_rom(file_path));

	// Initialize interpreter
	std::unique_ptr<chip8::Interpreter> interpreter = chip8::Interpreter::make_interpreter(std::move(memory_map));

	// Initialize SDL2 graphics
	chip8::Graphics::instance().init();

	// Game loop.
	for(;;)
	{
		// Proceed to next interpreter instruction
		interpreter->next_instruction();

		// Process key events
		interpreter->sync_keys( chip8::Graphics::instance().check_events() );

		// Update screen if required
		if( interpreter->draw() == true )
		{
			chip8::Graphics::instance().update_texture( interpreter->screen() );
		}

		// Delay for random time. Ideally use delay timer instead
		std::this_thread::sleep_for(std::chrono::microseconds(3600));
	}

	return 0;
}

// Chip8 fontset loaded into each rom at the start
const std::array<uint8_t, 80> FONTSET = 
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, //0
	0x20, 0x60, 0x20, 0x20, 0x70, //1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
	0x90, 0x90, 0xF0, 0x10, 0x10, //4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
	0xF0, 0x10, 0x20, 0x40, 0x40, //7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
	0xF0, 0x90, 0xF0, 0x90, 0x90, //A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
	0xF0, 0x80, 0x80, 0x80, 0xF0, //C
	0xE0, 0x90, 0x90, 0x90, 0xE0, //D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
	0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};


// Load data from file into memory map
std::unique_ptr<chip8::MemoryMap> load_rom(std::string rom_file_path)
{	
	std::unique_ptr<chip8::MemoryMap> memory_map = chip8::MemoryMap::makeMemoryMap(4096);

	// Load rom into memory map starting at 0
	unsigned int mem_adr = chip8::FONT_START;

	// Chip8 environment variables are static constants in Chip8Interpreter
	for(auto &font_byte : FONTSET)
	{
		memory_map->store( (std::byte) font_byte, mem_adr++);
	}

	// Lazy way of padding extra memory in map with default value 0
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
