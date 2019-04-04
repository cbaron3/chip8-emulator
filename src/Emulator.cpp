// Source file for implementing Emulator class functions
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <cstddef>
#include <string>

#include "../include/Chip8.h"
#include "../include/Emulator.h"
#include "../include/Memory.h"
#include "../include/Logger.h"
#include "../include/CPU.h"

namespace chip8{

	Emulator::Emulator(std::unique_ptr<MemoryMap> memory, std::unique_ptr<CPU> cpu) 
					  : memory_map(std::move(memory)), chip8_cpu(std::move(cpu))
	{
		// do nothing
	}

	// Load a rom from filepath fp into memory. First load fonts
	void Emulator::load_rom(std::string fp){
		// Destination vector for the bytes
		std::vector<uint8_t> v(FONTSET.begin(), FONTSET.end());
		std::vector<uint8_t> rom;

		// Open rom file
		std::ifstream f_rom(fp);
		
		// Read byte by byte and store in temporary vector to make sure we get all the bytes properly
		if(f_rom.is_open()){
			unsigned char c = f_rom.get();

			while(f_rom.good()){
				uint8_t data = (uint8_t) c;
				rom.push_back(data);
				c = f_rom.get();
			}
		}
		else{
			util::LOG(LOGTYPE::ERROR, "File: " + fp + " failed to open.");
		}

		// Close file
		f_rom.close();

		unsigned int mem_ptr = FONT_START;
		for(auto font_byte : v)
		{
			memory_map->store( (std::byte) font_byte, mem_ptr);
			mem_ptr+=1;
		}

		// Store vector in memory
		mem_ptr = PROG_START;
		for(auto rom_byte : rom)
		{
			memory_map->store( (std::byte) rom_byte, mem_ptr);
			++mem_ptr;
		}
		
	}

	void Emulator::print_memory(){
		std::cout << *memory_map;
	}

	void Emulator::next_instruction()
	{

		unsigned int pc = chip8_cpu->get_pc();

		// TODO: Clean up read short functionality into loose function?
		uint8_t first_byte = (uint8_t) memory_map->read(pc);
		prog_counter_++;
		uint8_t second_byte = (uint8_t) memory_map->read(pc+1);

		uint16_t opcode =  ((first_byte << 8) | (second_byte));
		
		if (pc >= MEM_SPACE){
			exit(0);
		}

		chip8_cpu->execute(opcode);

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
	}
}
