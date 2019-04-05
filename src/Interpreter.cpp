#include "../include/Interpreter.h"

#include <iostream>
#include "../include/Logger.h"
#include "../include/Chip8.h"
#include <string>
#include <algorithm>
#include <cstddef>
#include <bitset>
#include <random>
/* Module functions */


unsigned int extract_v( const unsigned int& in)
{
	return (in & 0xF000) >> 12;
}

/**
 * @brief      Extract register vx from input opcode
 *
 * @param[in]  in    opcode
 *
 * @return     Register vx
 */
unsigned int extract_vx(const unsigned int& in)
{
	return (in & 0x0F00) >> 8;
}

/**
 * @brief      Extract register vy from input opcode
 *
 * @param[in]  in    opcode
 *
 * @return     Register vy
 */
unsigned int extract_vy(const unsigned int& in)
{
	return (in & 0x00F0) >> 4;
}

/**
 * @brief      Extract value nnn from input opcode
 *
 * @param[in]  in    opcode
 *
 * @return     3 bit integer
 */
unsigned int extract_nnn(const unsigned int& in)
{
	return (in & 0x0FFF);
}

/**
 * @brief      Extract value nnn from input opcode
 *
 * @param[in]  in    opcode
 *
 * @return     2 bit integer
 */
unsigned int extract_nn(const unsigned int& in)
{
	return (in & 0x00FF);
}

/**
 * @brief      Extract value nnn from input opcode
 *
 * @param[in]  in    opcode
 *
 * @return     1 bit integer
 */
unsigned int extract_n(const unsigned int& in)
{
	return (in & 0x000F);
}		


/**
 * @brief      Converts a uint opcode to hex string
 *
 * @param[in]  opcode  The opcode
 */
std::string opcode_to_hex(const unsigned int& opcode)
{
	std::stringstream stream;
	stream << "0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << opcode;
	return stream.str();
}


namespace chip8
{
	// Default constructor that initializes members to default state
	Interpreter::Interpreter()
	{	
		m_program_counter = 0x200;
		m_exit_flag = false;
		m_draw_flag = false;

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
		
		std::fill ( &pixels[0][0],
	      		    &pixels[0][0] + sizeof(pixels) / sizeof(pixels[0][0]),
	      			false );
	}

	Interpreter::Interpreter( std::unique_ptr<MemoryMap> memory ) : Interpreter() 
	{ 
		memory_map = std::move(memory);
	}

	std::unique_ptr<Interpreter> Interpreter::make_interpreter( std::unique_ptr<MemoryMap> memory )
	{
		struct MakeUniquePublic : public Interpreter {
			MakeUniquePublic( std::unique_ptr<MemoryMap> memory ) : Interpreter(std::move(memory)) {}
      	};

		return std::make_unique<MakeUniquePublic>( std::move(memory) ); 
	}

	void Interpreter::next_instruction( void )
	{
		// Get opcode without modifying program counter
		unsigned int opcode =  ( 
								 ( ( unsigned int ) memory_map->read( m_program_counter ) << 8) | 
							     ( ( unsigned int ) memory_map->read( m_program_counter+1 ) ) 
							   );	
		
		execute(opcode);

		// Finally, update the timers
		if (m_delay_timer > 0)
		{
			// Will required some sort of sleep to synchronize 60hz delay with output of the emulator
			m_delay_timer -= 1;
		}

		if (m_sound_timer > 0)
		{
			// TODO: Sound left unimplemented for now
			m_sound_timer -= 1;
		}

		// TODO: Move out of here? Add boolean or move into each opcode or something
		m_program_counter += 2;
	}


	void Interpreter::execute( const unsigned int& opcode )
	{
		// Execute an opcode
		opcodes[extract_v(opcode)]( this, opcode );		
	}

	// Unit tested
	void Interpreter::opcode_0xxx( Interpreter* cpu, const unsigned int& opcode )
	{
		switch(extract_nn(opcode))
		{
			case 0x00E0:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Clear screen.");

				std::fill ( &cpu->pixels[0][0],
      		    			&cpu->pixels[0][0] + sizeof(cpu->pixels) / sizeof(cpu->pixels[0][0] ),
      						false );
				break;
			}
			case 0x00EE:
			{
				// TODO: std::to_string to print HEX
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Return from subroutine.");

				// To return from subroutine, pop from stack
				if(cpu->subroutine_stack.empty())
				{
					util::LOG(LOGTYPE::ERROR, "00EE return from subroutine stack underflow");
					cpu->m_exit_flag = true;
				}
				else
				{
					cpu->m_program_counter = ( cpu->subroutine_stack.top() - 2);
					cpu->subroutine_stack.pop();
				}

				break;
			}
			default:{
				util::LOG(LOGTYPE::ERROR, "Unknown opcode for 0xxx: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") ");
				break;
			}
		}
	}

	// Unit tested
	void Interpreter::opcode_1nnn( Interpreter* cpu, const unsigned int& opcode )
	{	
		std::string x = opcode_to_hex(opcode);

		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", Jump to address 1NNN.");
		cpu->m_program_counter = ( extract_nnn(opcode) );
		cpu->m_program_counter -= 2;	// Since execute +=2 the program counter and jump calls should prepare next instruction at absolute location. Nullify +=2 with a pre -=2
	}

	// Unit tested
	void Interpreter::opcode_2nnn( Interpreter* cpu, const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Call subroutine at 2NNN.");
		cpu->subroutine_stack.push( extract_nnn(opcode) );

		if(cpu->subroutine_stack.size() > 16)
		{
			util::LOG(LOGTYPE::ERROR, "2nnn call to subroutine stack overflow");
			cpu->m_exit_flag = true;
		}
	}
	
	// Unit tested
	void Interpreter::opcode_3xnn( Interpreter* cpu, const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Skip next instruct if Vx reg == kk at 3xkk.");

		if(cpu->registers[extract_vx(opcode)] == extract_nn(opcode))
		{
			cpu->m_program_counter += 2;
		}
	}
	
	// Unit tested
	void Interpreter::opcode_4xnn( Interpreter* cpu, const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Skip next instruct if Vx reg != kk at 4xkk.");
		unsigned int register_adr = ( extract_vx(opcode) );

		if( cpu->registers[register_adr] != extract_nn(opcode) )
		{
			cpu->m_program_counter += 2;
		}
	}
	
	// Unit tested
	void Interpreter::opcode_5xy0( Interpreter* cpu, const unsigned int& opcode )
	{	
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Skip next instruct if Vx reg == Vy reg at 5xy0.");

		if(cpu->registers[extract_vx(opcode)] == cpu->registers[extract_vy(opcode)])
			cpu->m_program_counter += 2;
	}
	
	// Unit tested
	void Interpreter::opcode_6xnn( Interpreter* cpu, const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set Vx = nn at 6xnn.");
		
		cpu->registers[extract_vx(opcode)] = extract_nn(opcode);
	}
	
	// Unit tested
	void Interpreter::opcode_7xnn( Interpreter* cpu, const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set Vx = Vx + kk at 7xkk.");

		cpu->registers[extract_vx(opcode)] += extract_nn(opcode);
	}
	
	// Unit tested
	void Interpreter::opcode_8XYx( Interpreter* cpu, const unsigned int& opcode )
	{
		unsigned int vx = extract_vx(opcode), vy = extract_vy(opcode);

		switch(opcode & 0x000F)
		{
			case 0x0000:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set Vx = Vy at 8xy0.");
				cpu->registers[vx] = cpu->registers[vy];
				break;	
			}
			case 0x0001:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set Vx = Vx or Vy at 8xy1.");
				cpu->registers[vx] |= cpu->registers[vy];
				break;	
			}
			case 0x0002:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set Vx = Vx and Vy at 8xy2.");
				cpu->registers[vx] &= cpu->registers[vy];
				break;	
			}
			case 0x0003:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set Vx = Vx XOR Vy at 8xy3.");
				cpu->registers[vx] ^= cpu->registers[vy];
				break;	
			}
			case 0x0004:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set Vx = Vx + Vy, set Vf = carry at 8xy4.");
				cpu->registers[vx] += cpu->registers[vy];


				// After adding the register contents, there needed to be a carry
				if(cpu->registers[vx] > 0xFF)	// Could use smaller width int but then still need to keep only lower 8 bits
				{	
					cpu->registers[vx] &= 0xFF; // Keep only lowest 8 bits if overflow
					cpu->registers[15] = 1; //carry
				}
				else
					cpu->registers[15] = 0;

				break;	
			}
			case 0x0005:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set Vx = Vx - Vy, set Vf = NOT borrow at 8xy5.");

				// There will be a borrow because vy is greater than vx
				if(cpu->registers[vy] > (cpu->registers[vx]))
				{					
					cpu->registers[15] = 1; //carry
				}
				else
				{
					cpu->registers[15] = 0;
				}

				cpu->registers[vx] -= cpu->registers[vy];
				cpu->registers[vx] &= 0xFF; // Keep only lowest 8 bits if overflow. TODO: Count backwards or 0?

				break;	
			}
			case 0x0006:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set Vx = Vx SHR 1 at 8xy6.");

				// If LSB of VX is 1, set carry
				cpu->registers[15] = (cpu->registers[vx] & 0x01);

				// Divide vx by 2
				cpu->registers[vx] >>= 1;

				break;	
			}
			case 0x0007:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set Vx = Vy - Vx, set Vf = NOT borrow at 8xy7.");

				// Borrow occurs when vx is greater than vy cause vy - vx will be negative
				if(cpu->registers[vy] > (cpu->registers[vx]))
					cpu->registers[15] = 1; // carry
				else
					cpu->registers[15] = 0; 
 
				cpu->registers[vx] = (cpu->registers[vy] - cpu->registers[vx]);
				cpu->registers[vx] &= 0xFF; // Keep only lowest 8 bits if overflow. TODO: Count backwards or 0?

				break;	
			}
			case 0x000E:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set Vx = Vx SHL 1 at 8xy8.");

				// If MSB of VX is 1, set carry
				cpu->registers[15] = cpu->registers[vx] >> 7; // Looks like error

				// Shift left by one
				cpu->registers[vx] <<= 1;
				break;	
			}
			default:
			{
				util::LOG(LOGTYPE::ERROR, "Unknown opcode for 8XYx: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") ");
				break;
			}
		}
	}
	
	// Unit tested
	void Interpreter::opcode_9xy0( Interpreter* cpu, const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Skip next instruct if Vx != Vy at 9xy0.");
		
		if( extract_vx(opcode) != extract_vy(opcode) )
			cpu->m_program_counter += 2;
	}
	
	void Interpreter::opcode_Annn( Interpreter* cpu, const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set I = nnn at Annn.");
		cpu->m_index_register = extract_nnn(opcode);
	}
	
	void Interpreter::opcode_Bxnn( Interpreter* cpu, const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Jump to nnn + V0 at Bnnn.");
		cpu->m_program_counter = ( extract_nnn(opcode) + cpu->registers[0]);
		cpu->m_program_counter -= 2;
	}
	
	void Interpreter::opcode_Cxnn( Interpreter* cpu, const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set Vx = rand byte AND kk Cxkk.");
		
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<float> dist(0.0, 255.0);

		cpu->registers[extract_vx(opcode)] = ((unsigned int) dist(mt)) & extract_nn(opcode);
	}
	
	void Interpreter::opcode_Dxyn( Interpreter* cpu, const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Display n byte sprite starting at mem loc I at (Vx, Vy), set Vf = collision at Dxyn.");
		// Sprite display
		// DIsplay nbyte sprite at memory location I at (vx,vy), if collision vf = collision
		
		// A sprite has a width of 8 pixels and a height of n pixels


		// Read n bytes from memory at location I
		unsigned int vx = extract_vx(opcode);
		unsigned int vy = extract_vy(opcode);
		unsigned int n =  extract_n(opcode);

		cpu->registers[15] = 0;

		for(int i = 0; i < n; i++)
		{
			unsigned int pixel_8bitrow = (uint8_t) cpu->memory_map->read(cpu->m_index_register + n);

			for(int j = 0; j < 8; j++)
			{
				// The x's and y's get confusing because vx is for y coord and vy is for x coord
				unsigned int x = (vy+j) % SCRN_HEIGHT;
				unsigned int y = (vx+i) % SCRN_WIDTH;

				// j represents position in bits as well. j = 0 means msb of pixel_8bitrow
				// CLEAR bits starting from MSB first every loop, working our way down to the LSB
				// pixel state is a boolean. on or off
				bool pixel_state = (pixel_8bitrow & (1 << (7-j)));

				// Set carry flag if a pixel is erased. The only time a pixel will be erased is if the pixel at vx,vy == pixel_state incoming
				if ( cpu->pixels[x][y] == pixel_state )
				{
					cpu->registers[15] = 1;
				} 

				cpu->pixels[x][y] ^= pixel_state;
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

				if(cpu->keys[cpu->registers[extract_vx(opcode)]] == true)
					cpu->m_program_counter += 2;

				break;	
			}
			case 0x00A1:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Skip next instrct if key with value Vx is not pressed at ExA1.");

				if(cpu->keys[cpu->registers[extract_vx(opcode)]] == false)
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

				cpu->registers[extract_vx(opcode)] = cpu->m_delay_timer;

				break;	
			}
			case 0x000A:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Wait for key press, store value of key in Vx at Fx0A.");
				// TODO: This seems inefficient ngl
				bool key_press = false;

				for(const bool& key : cpu->keys)
					key_press |= key;

				if(key_press == false)
					cpu->m_program_counter -= 2;

				break;	
			}
			case 0x0015:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set delay timer = Vx at Fx15.");

				cpu->m_delay_timer = cpu->registers[extract_vx(opcode)];
				break;	
			}
			case 0x0018:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set sound timer at Fx18.");
				
				cpu->m_sound_timer = cpu->registers[extract_vx(opcode)];
				break;	
			}
			case 0x001E:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set I = I + Vx at Fx1E.");

				// Set carry when overflow addition
				if(cpu->m_index_register + extract_vx(opcode) > 0xFFF)
				{
					cpu->registers[15] = 1;
				}
				else
				{
					cpu->registers[15] = 0;
				}

				// Add
				cpu->m_index_register += cpu->registers[extract_vx(opcode)];
				break;	
			}
			case 0x0029:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set I = location of sprite for digit Vx at Fx29.");
				// Vx stores a hexidecimal sprite 0x00 to 0x0F and they each take up 5 spots in memory
				unsigned int vx = extract_vx(opcode);
				cpu->m_index_register = cpu->registers[vx] * 5;
				break;	
			}
			case 0x0033:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set BCD rep of Vx in mem loc I, I+1, I+2 at Fx33.");
				// BCD means we need to take the up to 3 digit long value (max 255) and store each digit in a seperate memory location
				// Hundreds digit in I, tens in I+i, ones at I+2
				
				unsigned int vx_value = cpu->registers[extract_vx(opcode)];
				cpu->memory_map->store( (std::byte)(vx_value / 100)      , cpu->m_index_register, true);		// Hundreds. Divide by 100, left integer handle rounding
				cpu->memory_map->store( (std::byte)((vx_value / 10) % 10), cpu->m_index_register + 1, true);		// Tens. Divide by 10, then module base 10
				cpu->memory_map->store( (std::byte)(vx_value % 10)       , cpu->m_index_register + 2, true);		// Ones. Module base 10

				break;	
			}
			case 0x0055:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Store registers V0 through Vx in mem starting at loc I at Fx55.");
				unsigned int vx = extract_vx(opcode);
				unsigned int start_ir = cpu->ir();

				for(int i = 0; i <= vx; i++, start_ir++)
				{
					// Store register[i]
					cpu->memory_map->store( (std::byte) cpu->registers[i], start_ir, true);
				}

				break;	
			}
			case 0x0065:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Read registers V0 through Vx from mem starting at loc I at Fx65.");
				unsigned int vx = extract_vx(opcode);
				unsigned int start_ir = cpu->ir();

				for(int i = 0; i <= vx; i++, start_ir++)
				{
					// Read into register[i]
					cpu->registers[i] = (unsigned int) cpu->memory_map->read( start_ir );
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