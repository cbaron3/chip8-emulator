// Main file
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "../include/Emulator.h"
#include "../include/Memory.h"
#include "../include/Logger.h"


// TODO: (Carl Baron: Mar 29th): Pass memory, instruction set, and graphics renderer into Emulator class

std::vector<uint8_t> load_rom(std::string file_name);

int main(int argc, char **argv){
	std::cout << "Running main function..." << std::endl;
	util::Logger::getInstance()->set_max_log_level(LOGTYPE::DEBUG);

	chip8::Emulator emulator( chip8::MemoryMap::makeMemoryMap(4096, 0) );

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


// // for(;;)
/* {
		mem_adr = cpu.get_pc()

		opcode = memory.read(mem_adr)

		cpu.execute(mem_adr)

		if cpu.draw_flag() == true:
			graphics.render(cpu.get_pixels())

		cpu.update_key_input()

   }*/ 


// Emulator gets passed in a memory space and a cpu object and a graphics
// Every emulator loop, call cpu.getMemoryPointer and call memory.read() on that memorypointer to get the opcode

