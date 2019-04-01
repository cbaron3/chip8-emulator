// Main file
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "../include/CPU.h"

#include "../include/Emulator.h"
#include "../include/Memory.h"
#include "../include/Logger.h"


// TODO: (Carl Baron: Mar 29th): Pass instruction set, and graphics renderer into Emulator class

std::vector<uint8_t> load_rom(std::string file_name);

int main(int argc, char **argv){
	std::cout << "Running main function..." << std::endl;
	util::Logger::getInstance()->set_max_log_level(LOGTYPE::DEBUG);

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

	// Create emulator
	chip8::Emulator emulator( chip8::MemoryMap::makeMemoryMap(4096, 0), chip8::CPU::makeCPU());

	// TODO: Move load_fonts into load rom
	emulator.load_rom("../roms/test/TEST");
	emulator.print_memory();

	// Game loop
	for(;;)
	{
		emulator.next_instruction();
	}

	std::cout << "Ending main function..." << std::endl;
	return 0;
}

