// Main file
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "../include/Emulator.h"
#include "../include/Logger.h"

// TODO: (Carl Baron: Mar 29th): Pass memory, instruction set, and graphics renderer into Emulator class

std::vector<uint8_t> load_rom(std::string file_name);

int main(int argc, char **argv){
	std::cout << "Running main function..." << std::endl;
	util::Logger::getInstance()->set_max_log_level(LOGTYPE::DEBUG);

	chip8::Emulator emulator;

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

	emulator.load_fonts();
	emulator.print_memory();

	emulator.load_rom("../roms/full_games/PONG");
	emulator.print_memory();


	// Game loop
	while(true){
		emulator.execute_opcode(emulator.fetch_opcode());
		//break;
	}

	std::cout << "Ending main function..." << std::endl;
	return 0;
}
