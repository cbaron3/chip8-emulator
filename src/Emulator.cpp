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

namespace chip8{
	// Load font set into memory
	void Emulator::load_fonts(){
		// Convert fontset array into vector
		std::vector<uint8_t> v(FONTSET.begin(), FONTSET.end());
		

		unsigned int mem_ptr = FONT_START;
		for(auto font_byte : v)
		{
			memory.store(mem_ptr, (std::byte) font_byte);
			mem_ptr+=1;
		}
	}

	// Load a rom from filepath fp into memory
	void Emulator::load_rom(std::string fp){
		// Destination vector for the bytes
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

		// Store vector in memory
		unsigned int mem_ptr = PROG_START;
		for(auto rom_byte : rom)
		{
			memory.store(mem_ptr, (std::byte) rom_byte);
			++mem_ptr;
		}
		
	}

	// TODO: (Carl Baron: Feb 2nd): Badly designed interface between the two classes
	void Emulator::print_memory(){
		std::cout << memory;
	}

	uint16_t Emulator::fetch_opcode(){
		uint8_t first_byte = (uint8_t) memory.read(prog_counter_);
		prog_counter_++;
		uint8_t second_byte = (uint8_t) memory.read(prog_counter_);
		prog_counter_++;

		uint16_t s =  ((first_byte << 8) | (second_byte));

		if (prog_counter_ >= MEM_SPACE){
			exit(0);
		}
		
		return s;
	}

	void Emulator::execute_opcode(uint16_t opcode){
		// First check the first byte to simplify the decision on which opcode to select	

		switch(opcode & 0xF000)
		{
			case 0x0000:
			{
				switch(opcode & 0x00FF)
				{
					case 0x00E0:
					{
						util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Clear screen.");
						break;
					}
					case 0x00EE:
					{
						util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Return from subroutine.");
						break;
					}
					default:{
						util::LOG(LOGTYPE::ERROR, "Unknown opcode: " + std::to_string(opcode));
						break;
					}
				}
				break;
			}

			case 0x1000:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Jump to address 1NNN.");
				break;
			}

			case 0x2000:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Call subroutine at 2NNN.");
				break;
			}

			case 0x3000:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Skip next instruct if Vx reg == kk at 3xkk.");
				break;			
			}

			case 0x4000:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Skip next instruct if Vx reg != kk at 3xkk.");
				break;			
			}

			case 0x5000:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Skip next instruct if Vx reg == Vy reg at 5xy0.");
				break;			
			}

			case 0x6000:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Set Vx = kk at 6xkk.");
				break;			
			}

			case 0x7000:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Set Vx = Vx + kk at 7xkk.");
				break;			
			}

			case 0x8000:
			{
				// Only compare based on last byte
				switch(opcode & 0x000F)
				{
					case 0x0000:
					{
						util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Set Vx = Vy at 8xy0.");
						break;	
					}
					case 0x0001:
					{
						util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Set Vx = Vx or Vy at 8xy1.");
						break;	
					}
					case 0x0002:
					{
						util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Set Vx = Vx and Vy at 8xy2.");
						break;	
					}
					case 0x0003:
					{
						util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Set Vx = Vx XOR Vy at 8xy3.");
						break;	
					}
					case 0x0004:
					{
						util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Set Vx = Vx + Vy, set Vf = carry at 8xy4.");
						break;	
					}
					case 0x0005:
					{
						util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Set Vx = Vx - Vy, set Vf = NOT borrow at 8xy5.");
						break;	
					}
					case 0x0006:
					{
						util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Set Vx = Vx SHR 1 at 8xy6.");
						break;	
					}
					case 0x0007:
					{
						util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Set Vx = Vy - Vx, set Vf = NOT borrow at 8xy7.");
						break;	
					}
					case 0x000E:
					{
						util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Set Vx = Vx SHL 1 at 8xy8.");
						break;	
					}
					default:
					{
						util::LOG(LOGTYPE::ERROR, "Unknown opcode: " + std::to_string(opcode));
						break;
					}
				}
				break;
			}
			case 0x9000:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Skip next instruct if Vx != Vy at 9xy0.");
				break;	
			}
			case 0xA000:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Set I = nnn at Annn.");
				break;	
			}
			case 0xB000:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Jump to nnn + V0 at Bnnn.");
				break;	
			}
			case 0xC000:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Set Vx = rand byte AND kk Cxkk.");
				break;	
			}
			case 0xD000:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Display n byte sprite starting at mem loc I at (Vx, Vy), set Vf = collision at Dxyn.");
				break;	
			}
			case 0xE000:
			{
				switch(opcode & 0x00FF)
				{
					case 0x009E:{
						util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Skip next instrct if key with value Vx is pressed at Ex9E.");
						break;	
					}
					case 0x00A1:
					{
						util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Skip next instrct if key with value Vx is not pressed at ExA1.");
						break;	
					}
					default:
					{
						util::LOG(LOGTYPE::ERROR, "Unknown opcode: " + std::to_string(opcode));
						break;
					}
				}	
			}
			case 0xF000:
			{
				switch(opcode & 0x00FF){
					case 0x0007:
					{
						util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Set Vx = delay time value at Fx07.");
						break;	
					}
					case 0x000A:
					{
						util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Wait for ley press, store value of key in Vx at Fx0A.");
						break;	
					}
					case 0x0015:
					{
						util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Set delay timer = Vx at Fx15.");
						break;	
					}
					case 0x0018:
					{
						util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Set sound timer at Fx18.");
						break;	
					}
					case 0x001E:
					{
						util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Set I = I + Vx at Fx1E.");
						break;	
					}
					case 0x0029:
					{
						util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Set I = location of sprite for digit Vx at Fx29.");
						break;	
					}
					case 0x0033:
					{
						util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Set BCD rep of Vx in mem loc I, I+1, I+2 at Fx33.");
						break;	
					}
					case 0x0055:
					{
						util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Store registers V0 through Vx in mem starting at loc I at Fx55.");
						break;	
					}
					case 0x0065:
					{
						util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Read registers V0 through Vx from mem starting at loc I at Fx65.");
						break;	
					}
					default:
					{
						util::LOG(LOGTYPE::ERROR, "Unknown opcode: " + std::to_string(opcode));
						break;
					}
				}
			}
			
			default:
			{
				util::LOG(LOGTYPE::ERROR, "Unknown opcode: " + std::to_string(opcode));
				break;
			}
		}
	}
}
