#include "../include/Interpreter.h"

#include <iostream>
#include "../include/Logger.h"
#include "../include/Chip8.h"
#include <string>
#include <algorithm>
#include <cstddef>
#include <bitset>
#include <random>

namespace	/* Module functions */
{
// Used for debugging opcodes
std::string opcode_to_hex(const unsigned int &opcode)
{
	std::stringstream stream;
	stream << "0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << opcode;
	return stream.str();
}

// Used for extracting bit fields from opcodes
unsigned int _v(const unsigned int &in) { return (in & 0xF000) >> 12; }
unsigned int _vx(const unsigned int &in) { return (in & 0x0F00) >> 8; }
unsigned int _vy(const unsigned int &in) { return (in & 0x00F0) >> 4; }
unsigned int _nnn(const unsigned int &in) { return (in & 0x0FFF); }
unsigned int _nn(const unsigned int &in) { return (in & 0x00FF); }
unsigned int _n(const unsigned int &in) { return (in & 0x000F); }
} // anonymous namespace

namespace chip8
{
	// Default constructor that initializes members to default state
	Interpreter::Interpreter()
	{	
		// Inital program counter value
		m_program_counter = 0x200;

		// Other registers
		m_delay_timer = 0x0;
		m_sound_timer = 0x0;
		m_index_register = 0x0;
		m_sp = 0x0;

		// Container initialization
		m_pixels = {};
		m_stack = {};
		m_keys = {};
		m_registers = {};
		
		// Draw and exit flag
		m_exit_flag = false;
		m_draw_flag = false;

		// Opcode function table
		opcodes[0] =  opcode_0xxx;
		opcodes[1] =  opcode_1nnn; 	
		opcodes[2] =  opcode_2nnn;
		opcodes[3] =  opcode_3xnn; 
		opcodes[4] =  opcode_4xnn; 
		opcodes[5] =  opcode_5xy0;
		opcodes[6] =  opcode_6xnn;
		opcodes[7] =  opcode_7xnn;
		opcodes[8] =  opcode_8XYx;
		opcodes[9] =  opcode_9xy0;
		opcodes[10] = opcode_Annn;
		opcodes[11] = opcode_Bxnn;
		opcodes[12] = opcode_Cxnn;
		opcodes[13] = opcode_Dxyn;
		opcodes[14] = opcode_EXxx;
		opcodes[15] = opcode_FXxx;
	}

	// Overloaded constructor
	Interpreter::Interpreter( std::unique_ptr<MemoryMap> memory ) : Interpreter() 
	{ 
		// Move the unique memory map into this
		memory_map = std::move(memory);
	}

	// Factory method
	// Uses local struct to dodge private constructor issue for static method
	std::unique_ptr<Interpreter> Interpreter::make_interpreter( std::unique_ptr<MemoryMap> memory )
	{
		struct MakeUniquePublic : public Interpreter {
			MakeUniquePublic( std::unique_ptr<MemoryMap> memory ) : Interpreter(std::move(memory)) {}
      	};

		return std::make_unique<MakeUniquePublic>( std::move(memory) ); 
	}

	// Execute next instruction
	void Interpreter::next_instruction( void )
	{
		// Get opcode without modifying program counter
		unsigned int opcode = (((unsigned int)memory_map->read(m_program_counter) << 8) |
													 ((unsigned int)memory_map->read(m_program_counter + 1)));
		m_program_counter += 2;
		execute(opcode);

		// Timer update (Not used)
		if (m_delay_timer > 0)
			m_delay_timer -= 1;

		if (m_sound_timer > 0)
			m_sound_timer -= 1;
	}

	// Separate next_instruction and execute so we can unit test individual opcodes
	void Interpreter::execute( const unsigned int& opcode )
	{
		// Execute an opcode
		opcodes[_v(opcode)]( this, opcode );		
	}

	// Unit tested
	void Interpreter::opcode_0xxx( Interpreter* cpu, const unsigned int& opcode )
	{
		switch(_nn(opcode))
		{
			// Clear screen
			case 0x00E0:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Clear screen.");
				cpu->m_pixels.fill(0);
			} break;
			// Return from subroutine
			case 0x00EE:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Return from subroutine.");
				cpu->m_program_counter = cpu->m_stack[--cpu->m_sp];
			} break;
			// Unknown opcode
			default:{
				util::LOG(LOGTYPE::ERROR, "Unknown opcode for 0xxx: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") ");
			} break;
		}
	}

	// Unit tested
	void Interpreter::opcode_1nnn( Interpreter* cpu, const unsigned int& opcode )
	{
		// Jump to address NNN
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", Jump to address 1NNN.");
		cpu->m_program_counter = ( _nnn(opcode) );
	}

	// Unit tested
	void Interpreter::opcode_2nnn( Interpreter* cpu, const unsigned int& opcode )
	{
		// Call subroutine at NNN
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Call subroutine at 2NNN.");
		cpu->m_stack[cpu->m_sp++] = cpu->m_program_counter;
		cpu->m_program_counter = _nnn(opcode);

		if (cpu->m_sp >= 16) {
			util::LOG(LOGTYPE::ERROR, "Stack overflow");
			cpu->m_exit_flag = true;
		}
	}
	
	// Unit tested
	void Interpreter::opcode_3xnn( Interpreter* cpu, const unsigned int& opcode )
	{
		// Skip next instruction if VX == NN
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Skip next instruct if Vx reg == kk at 3xkk.");
		if(cpu->m_registers[_vx(opcode)] == _nn(opcode))
			cpu->m_program_counter += 2;
	}
	
	// Unit tested
	void Interpreter::opcode_4xnn( Interpreter* cpu, const unsigned int& opcode )
	{
		// Skip next instruction if VX != NN
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Skip next instruct if Vx reg != kk at 4xkk.");
		if( cpu->m_registers[_vx(opcode)] != _nn(opcode) )
			cpu->m_program_counter += 2;
	}
	
	// Unit tested
	void Interpreter::opcode_5xy0( Interpreter* cpu, const unsigned int& opcode )
	{	
		// Skip next instruction if VX == VY
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Skip next instruct if Vx reg == Vy reg at 5xy0.");
		if(cpu->m_registers[_vx(opcode)] == cpu->m_registers[_vy(opcode)])
			cpu->m_program_counter += 2;
	}
	
	// Unit tested
	void Interpreter::opcode_6xnn( Interpreter* cpu, const unsigned int& opcode )
	{
		// Set VX = NN
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set Vx = nn at 6xnn.");
		cpu->m_registers[_vx(opcode)] = _nn(opcode)&0x00FF;
	}
	
	// Unit tested
	void Interpreter::opcode_7xnn( Interpreter* cpu, const unsigned int& opcode )
	{
		// Set VX = VX + NN
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set Vx = Vx + kk at 7xkk.");
		cpu->m_registers[_vx(opcode)] += _nn(opcode)&0x00FF;
	}
	
	// Unit tested
	void Interpreter::opcode_8XYx( Interpreter* cpu, const unsigned int& opcode )
	{
		// Reduce function call necessity 
		unsigned int vx = _vx(opcode), vy = _vy(opcode);

		switch(opcode & 0x000F)
		{
			case 0x0000:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set Vx = Vy at 8xy0.");
				cpu->m_registers[vx] = cpu->m_registers[vy];
					
			} break;
			case 0x0001:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set Vx = Vx or Vy at 8xy1.");
				cpu->m_registers[vx] |= cpu->m_registers[vy];
					
			} break;
			case 0x0002:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set Vx = Vx and Vy at 8xy2.");
				cpu->m_registers[vx] &= cpu->m_registers[vy];
					
			} break;
			case 0x0003:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set Vx = Vx XOR Vy at 8xy3.");
				cpu->m_registers[vx] ^= cpu->m_registers[vy];
					
			} break;
			case 0x0004:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set Vx = Vx + Vy, set Vf = carry at 8xy4.");
				
				// After adding the register contents, there needed to be a carry
				if(cpu->m_registers[vy] + cpu->m_registers[vx] > 0xFF)	// Could use smaller width int but then still need to keep only lower 8 bits
				{	
					cpu->m_registers[15] = 1; //carry
				}
				else
					cpu->m_registers[15] = 0;

				cpu->m_registers[vx] += cpu->m_registers[vy];

			} break;
			case 0x0005:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set Vx = Vx - Vy, set Vf = NOT borrow at 8xy5.");
	
				cpu->m_registers[15] = cpu->m_registers[vx] > (cpu->m_registers[vy]); //carry
				cpu->m_registers[vx] -= cpu->m_registers[vy];

			} break;
			case 0x0006:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set Vx = Vx SHR 1 at 8xy6.");

				// If LSB of VX is 1, set carry
				cpu->m_registers[15] = (cpu->m_registers[vx] & 0x01);

				// Divide vx by 2
				cpu->m_registers[vx] = cpu->m_registers[vy] >> 1;

			} break;
			case 0x0007:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set Vx = Vy - Vx, set Vf = NOT borrow at 8xy7.");

				// Borrow occurs when vx is greater than vy cause vy - vx will be negative
				if(cpu->m_registers[vy] > (cpu->m_registers[vx]))
					cpu->m_registers[15] = 1; // carry
				else
					cpu->m_registers[15] = 0; 
 
				cpu->m_registers[vx] = (cpu->m_registers[vy] - cpu->m_registers[vx]);
					
			} break;
			case 0x000E:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set Vx = Vx SHL 1 at 8xy8.");

				// If MSB of VX is 1, set carry
				cpu->m_registers[15] = (cpu->m_registers[vx]&0x80) >> 7; // Looks like error

				// Shift left by one
				cpu->m_registers[vx] = (cpu->m_registers[vy] << 1) & 0xFF;
					
			} break;
			default:
			{
				util::LOG(LOGTYPE::ERROR, "Unknown opcode for 8XYx: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") ");
			} break;
		}
	}
	
	// Unit tested
	void Interpreter::opcode_9xy0( Interpreter* cpu, const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Skip next instruct if Vx != Vy at 9xy0.");
		
		if( cpu->m_registers[_vx(opcode)] != cpu->m_registers[_vy(opcode)] )
			cpu->m_program_counter += 2;
	}
	
	void Interpreter::opcode_Annn( Interpreter* cpu, const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set I = nnn at Annn.");
		cpu->m_index_register = _nnn(opcode);
		util::LOG(LOGTYPE::DEBUG, "Index register is now: " + std::to_string(_nnn(opcode)));
	}
	
	void Interpreter::opcode_Bxnn( Interpreter* cpu, const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Jump to nnn + V0 at Bnnn.");
		cpu->m_program_counter = ( _nnn(opcode) + cpu->m_registers[0]);
	}
	
	void Interpreter::opcode_Cxnn( Interpreter* cpu, const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set Vx = rand byte AND kk Cxkk.");
		
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<float> dist(0.0, 256.0);

		cpu->m_registers[_vx(opcode)] = ((unsigned int) dist(mt)) & _nn(opcode);
	}
	
	void Interpreter::opcode_Dxyn( Interpreter* cpu, const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Display n byte sprite starting at mem loc I at (Vx, Vy), set Vf = collision at Dxyn.");
		// Sprite display
		// DIsplay nbyte sprite at memory location I at (vx,vy), if collision vf = collision
		
		// A sprite has a width of 8 pixels and a height of n pixels


		// Read n bytes from memory at location I
		unsigned int Vx = cpu->m_registers[_vx(opcode)];
		unsigned int Vy = cpu->m_registers[_vy(opcode)];
		unsigned int height = _n(opcode);

		unsigned short pixel;

		cpu->m_registers[15] = 0;
		for(int y = 0; y < height; ++y)
		{
			pixel = (unsigned short) cpu->memory_map->read(cpu->m_index_register + y);

			for(int x = 0; x < 8; ++x)
			{
				if((pixel & (0x80 >> x)) != 0)
                    {	
                    	int px = ((Vx + x) & 63);
						int py = ((Vy + y) & 31);

						int pixelPos = (64 * py) + px;

                        cpu->m_registers[15] = (cpu->m_pixels[pixelPos] > 0) & (pixel);

                        cpu->m_pixels[pixelPos] ^= 0xFFFFFFFF;
                    }
										else
										{
											                    	int px = ((Vx + x) & 63);
						int py = ((Vy + y) & 31);

						int pixelPos = (64 * py) + px;

                        cpu->m_registers[15] = (cpu->m_pixels[pixelPos] > 0) & (pixel);

                        cpu->m_pixels[pixelPos] ^= 0;
										}
										
			}
		}
	
		cpu->m_draw_flag = true;
	}
	
	void Interpreter::opcode_EXxx( Interpreter* cpu, const unsigned int& opcode )
	{
		switch(opcode & 0x00FF)
		{
			case 0x009E:{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Skip next instrct if key with value Vx is pressed at Ex9E.");

				if(cpu->m_keys[cpu->m_registers[_vx(opcode)]] == true)
					cpu->m_program_counter += 2;

				break;	
			}
			case 0x00A1:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Skip next instrct if key with value Vx is not pressed at ExA1.");

				if(cpu->m_keys[cpu->m_registers[_vx(opcode)]] == false)
					cpu->m_program_counter += 2;

				break;	
			}
			default:
			{
				util::LOG(LOGTYPE::ERROR, "Unknown opcode for 9xy0: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") ");
				break;
			}
		}
	}
	
	void Interpreter::opcode_FXxx( Interpreter* cpu, const unsigned int& opcode )
	{
		switch(opcode & 0x00FF)
		{
			case 0x0007:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set Vx = delay time value at Fx07.");

				cpu->m_registers[_vx(opcode)] = cpu->m_delay_timer;

				break;	
			}
			case 0x000A:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Wait for key press, store value of key in Vx at Fx0A.");
				bool key_press = false;

				for(const bool& key : cpu->m_keys)
				{

					key_press |= key;
				}

				if(key_press == false)
					cpu->m_program_counter -= 2;

				break;	
			}
			case 0x0015:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set delay timer = Vx at Fx15.");

				cpu->m_delay_timer = cpu->m_registers[_vx(opcode)];
				break;	
			}
			case 0x0018:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set sound timer at Fx18.");
				
				cpu->m_sound_timer = cpu->m_registers[_vx(opcode)];
				break;	
			}
			case 0x001E:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set I = I + Vx at Fx1E.");

				// Add
				cpu->m_index_register = (cpu->m_index_register + cpu->m_registers[_vx(opcode)])&0xFFFF;
				break;	
			}
			case 0x0029:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set I = location of sprite for digit Vx at Fx29.");
				// Vx stores a hexidecimal sprite 0x00 to 0x0F and they each take up 5 spots in memory
				unsigned int vx = _vx(opcode);
				cpu->m_index_register = cpu->m_registers[vx] * 5;
				break;	
			}
			case 0x0033:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set BCD rep of Vx in mem loc I, I+1, I+2 at Fx33.");
				// BCD means we need to take the up to 3 digit long value (max 255) and store each digit in a seperate memory location
				// Hundreds digit in I, tens in I+i, ones at I+2
				
				unsigned int vx_value = cpu->m_registers[_vx(opcode)];
				cpu->memory_map->store( (std::byte)(vx_value / 100)      , cpu->m_index_register, true);		// Hundreds. Divide by 100, left integer handle rounding
				cpu->memory_map->store( (std::byte)((vx_value / 10) % 10), cpu->m_index_register + 1, true);		// Tens. Divide by 10, then module base 10
				cpu->memory_map->store( (std::byte)(vx_value % 10)       , cpu->m_index_register + 2, true);		// Ones. Module base 10

				break;	
			}
			case 0x0055:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Store m_registers V0 through Vx in mem starting at loc I at Fx55.");
				unsigned int vx = _vx(opcode);

				for(int i = 0; i <= vx; ++i)
				{
					// Store register[i]
					cpu->memory_map->store( (std::byte) cpu->m_registers[i], cpu->m_index_register+i, true);
				}


				break;	
			}
			case 0x0065:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Read m_registers V0 through Vx from mem starting at loc I at Fx65.");
				unsigned int vx = _vx(opcode);

				for(int i = 0; i <= vx; ++i)
				{
					// Read into register[i]
					cpu->m_registers[i] = (uint8_t) cpu->memory_map->read( cpu->m_index_register + i );
				}


				break;	
			}
			default:
			{
				util::LOG(LOGTYPE::ERROR, "Unknown opcode for FXxx: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") ");
				break;
			}
		}
	}
} // end of chip8 namespace