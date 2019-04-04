#include "../include/CPU.h"

#include <iostream>
#include "../include/Logger.h"
#include <string>
/* Module functions */

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
	CPU::CPU()
	{	
		prog_counter = 0x200;
		exit_flag = false;

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
	      			0 );
	}

	std::unique_ptr<CPU> CPU::makeCPU()
	{
		struct MakeUniquePublic : public CPU {};
		return std::make_unique<MakeUniquePublic>(); 
	}

	void CPU::execute( const unsigned int& opcode)
	{
		// Extract MSB nibble which contains opcode specifier
		unsigned int opcode_to_execute = (opcode & 0xF000) >> 12;

		// Call the appropriate opcode
		// TODO: Add safer opcode handling
		if(opcode_to_execute < 0 || opcode_to_execute > 15)
		{
			// do nothing
		}
		else
		{
			opcodes[opcode_to_execute]( this, opcode );	
		}

		// Finally, update the timers
		if (delay_timer > 0)
		{
			// Will required some sort of sleep to synchronize 60hz delay with output of the emulator
			delay_timer -= 1;
		}

		if (sound_timer > 0)
		{
			// TODO: Sound left unimplemented for now
			sound_timer -= 1;
		}

		prog_counter += 2;
		
	}

	void CPU::print_pixels( void ) const
	{
		// initialize elements
		  for (auto & outer_array : pixels)  
		  {
		      for(auto & inner_array : outer_array)
		      {
		      	std::cout << inner_array << ' ';
		      }

		      std::cout << std::endl;
		  }
	}

	void CPU::print_subr_stack( void ) const
	{
		std::stack<unsigned int> temp_stack = subroutine_stack;

		std::cout << " ########## STACK ##########" << std::endl;
		std::cout << "Size: " << temp_stack.size() << std::endl;
		while(!temp_stack.empty()) //body
	    {
	        std::cout << temp_stack.top() << " ";
	        temp_stack.pop();
	    }
	    std::cout << " \n########## END ##########" << std::endl;
	}

	void CPU::opcode_0xxx( CPU* cpu, const unsigned int& opcode )
	{
		switch(opcode & 0x00FF)
		{
			case 0x00E0:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Clear screen.");

				std::fill ( &cpu->pixels[0][0],
      		    			&cpu->pixels[0][0] + sizeof(cpu->pixels) / sizeof(cpu->pixels[0][0] ),
      						0 );
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
					cpu->set_exit_flag(true);
				}
				else
				{
					cpu->set_pc( cpu->subroutine_stack.top() - 2);
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

	void CPU::opcode_1nnn( CPU* cpu, const unsigned int& opcode )
	{	
		std::string x = opcode_to_hex(opcode);

		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", Jump to address 1NNN.");
		cpu->set_pc( opcode & 0x0FFF );
		cpu->set_pc( cpu->get_pc() - 2);	// Since execute +=2 the program counter and jump calls should prepare next instruction at absolute location. Nullify +=2 with a pre -=2
	}

	void CPU::opcode_2nnn( CPU* cpu, const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Call subroutine at 2NNN.");
		cpu->subroutine_stack.push(opcode & 0x0FFF);

		if(cpu->subroutine_stack.size() > 16)
		{
			util::LOG(LOGTYPE::ERROR, "2nnn call to subroutine stack overflow");
			cpu->set_exit_flag(true);
		}
	}
	
	void CPU::opcode_3xnn( CPU* cpu, const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Skip next instruct if Vx reg == kk at 3xkk.");

		if(cpu->registers[extract_vy(opcode)] == extract_nn(opcode))
		{
			cpu->set_pc( cpu->get_pc() + 2 );
		}
	}
	
	void CPU::opcode_4xnn( CPU* cpu, const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Skip next instruct if Vx reg != kk at 4xkk.");
		unsigned int register_adr = (opcode & 0x0F00) >> 8;

		if(cpu->registers[register_adr] != extract_nn(opcode))
		{
			cpu->set_pc( cpu->get_pc() + 2 );
		}
	}
	
	void CPU::opcode_5xy0( CPU* cpu, const unsigned int& opcode )
	{	
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Skip next instruct if Vx reg == Vy reg at 5xy0.");
		unsigned int vx = (opcode & 0x0F00) >> 8;
		unsigned int vy = (opcode & 0x00F0) >> 4;

		if(cpu->registers[vx] == cpu->registers[vy])
		{
			cpu->set_pc( cpu->get_pc() + 2 );
		}
		
	}
	
	void CPU::opcode_6xnn( CPU* cpu, const unsigned int& opcode )
	{
		//std::string test = util::opcode_to_hex(opcode);

		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set Vx = kk at 6xkk.");

		unsigned int vx = (opcode & 0x0F00) >> 8;
		unsigned int kk = (opcode & 0x00FF);

		cpu->registers[vx] = kk;

	}
	
	void CPU::opcode_7xnn( CPU* cpu, const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set Vx = Vx + kk at 7xkk.");

		unsigned int vx = (opcode & 0x0F00) >> 8;
		unsigned int kk = (opcode & 0x00FF);

		cpu->registers[vx] += kk;
	}
	
	void CPU::opcode_8XYx( CPU* cpu, const unsigned int& opcode )
	{
		unsigned int vx = (opcode & 0x0F00) >> 8;
		unsigned int vy = (opcode & 0x00F0) >> 4;

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
				cpu->registers[vx] >>= 1;	// TODO: looks sketchy to me, may use shift right 1

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
	
	void CPU::opcode_9xy0( CPU* cpu, const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Skip next instruct if Vx != Vy at 9xy0.");
		unsigned int vx = (opcode & 0x0F00) >> 8;
		unsigned int vy = (opcode & 0x00F0) >> 4;

		if(vx == vy)
		{
			cpu->set_pc( cpu->get_pc() + 2 );
		}
	}
	
	void CPU::opcode_Annn( CPU* cpu, const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set I = nnn at Annn.");
		cpu->index_register = opcode & 0x0FFF;
	}
	
	void CPU::opcode_Bxnn( CPU* cpu, const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Jump to nnn + V0 at Bnnn.");
		cpu->set_pc( opcode & 0x0FFF + cpu->registers[0]);
		cpu->set_pc( cpu->get_pc() - 2);

	}
	
	void CPU::opcode_Cxnn( CPU* cpu, const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set Vx = rand byte AND kk Cxkk.");
		// TODO: Use true random numbers c++ hash engine thingy
	}
	
	void CPU::opcode_Dxyn( CPU* cpu, const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Display n byte sprite starting at mem loc I at (Vx, Vy), set Vf = collision at Dxyn.");
	}
	
	void CPU::opcode_EXxx( CPU* cpu, const unsigned int& opcode )
	{
		switch(opcode & 0x00FF)
		{
			case 0x009E:{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Skip next instrct if key with value Vx is pressed at Ex9E.");
				unsigned int vx = (opcode & 0x0F00) >> 8;

				if(cpu->keys[cpu->registers[vx]] == true)
				{
					cpu->set_pc( cpu->get_pc() + 2 );
				}

				break;	
			}
			case 0x00A1:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Skip next instrct if key with value Vx is not pressed at ExA1.");
				unsigned int vx = (opcode & 0x0F00) >> 8;

				if(cpu->keys[cpu->registers[vx]] == false)
				{
					cpu->set_pc( cpu->get_pc() + 2 );
				}

				break;	
			}
			default:
			{
				util::LOG(LOGTYPE::ERROR, "Unknown opcode for 9xy0: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") ");
				break;
			}
		}
	}
	
	void CPU::opcode_FXxx( CPU* cpu, const unsigned int& opcode )
	{
		switch(opcode & 0x00FF)
		{
			case 0x0007:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set Vx = delay time value at Fx07.");
				unsigned int vx = (opcode & 0x0F00) >> 8;

				cpu->registers[vx] = cpu->delay_timer;

				break;	
			}
			case 0x000A:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Wait for key press, store value of key in Vx at Fx0A.");
				// TODO: Looks like this is the only operation that needs to wait for prog counter. Either make prog counter increment on all other cases or implement wait for key boolean?
				cpu->set_pc(cpu->get_pc()-2);	// Does this solve the problem? Enter execute, go to this opcode, decrement pc, increment same amount before leaving execute. Net pc change is 0 and does not affect others?
				// Just need wait for key press functionality?
				// Wrap pc -= 2 in if statement checking for key in key array
				break;	
			}
			case 0x0015:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set delay timer = Vx at Fx15.");
				unsigned int vx = (opcode & 0x0F00) >> 8;

				cpu->delay_timer = cpu->registers[vx];
				break;	
			}
			case 0x0018:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set sound timer at Fx18.");
				unsigned int vx = (opcode & 0x0F00) >> 8;
				
				cpu->sound_timer = cpu->registers[vx];
				break;	
			}
			case 0x001E:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set I = I + Vx at Fx1E.");
				unsigned int vx = (opcode & 0x0F00) >> 8;

				// Set carry when overflow addition
				if(cpu->index_register + vx > 0xFFF)
				{
					cpu->registers[15] = 1;
				}
				else
				{
					cpu->registers[15] = 0;
				}

				// Add
				cpu->index_register += cpu->registers[vx];
				break;	
			}
			case 0x0029:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set I = location of sprite for digit Vx at Fx29.");
				break;	
			}
			case 0x0033:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Set BCD rep of Vx in mem loc I, I+1, I+2 at Fx33.");
				break;	
			}
			case 0x0055:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Store registers V0 through Vx in mem starting at loc I at Fx55.");
				break;	
			}
			case 0x0065:
			{
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + opcode_to_hex(opcode) + ", (" + opcode_to_hex(opcode) + ", (" + std::to_string(opcode) + ")" + ") " + ", Read registers V0 through Vx from mem starting at loc I at Fx65.");
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