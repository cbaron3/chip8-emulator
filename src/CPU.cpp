#include "../include/CPU.h"

#include <iostream>
#include "../include/Logger.h"

namespace chip8
{
	CPU::CPU()
	{
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
			opcodes[opcode_to_execute]( opcode );	
		}
		
	}

	void CPU::opcode_0xxx( const unsigned int& opcode )
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
				// TODO: std::to_string to print HEX
				util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Return from subroutine.");
				break;
			}
			default:{
				util::LOG(LOGTYPE::ERROR, "Unknown opcode for 0xxx: " + std::to_string(opcode));
				break;
			}
		}
	}

	void CPU::opcode_1nnn( const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Jump to address 1NNN.");
	}

	void CPU::opcode_2nnn( const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Call subroutine at 2NNN.");
	}
	
	void CPU::opcode_3xnn( const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Skip next instruct if Vx reg == kk at 3xkk.");
	}
	
	void CPU::opcode_4xnn( const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Skip next instruct if Vx reg != kk at 4xkk.");
	}
	
	void CPU::opcode_5xy0( const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Skip next instruct if Vx reg == Vy reg at 5xy0.");
	}
	
	void CPU::opcode_6xnn( const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Set Vx = kk at 6xkk.");
	}
	
	void CPU::opcode_7xnn( const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Set Vx = Vx + kk at 7xkk.");
	}
	
	void CPU::opcode_8XYx( const unsigned int& opcode )
	{
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
				util::LOG(LOGTYPE::ERROR, "Unknown opcode for 8XYx: " + std::to_string(opcode));
				break;
			}
		}
	}
	
	void CPU::opcode_9xy0( const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Skip next instruct if Vx != Vy at 9xy0.");
	}
	
	void CPU::opcode_Annn( const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Set I = nnn at Annn.");
	}
	
	void CPU::opcode_Bxnn( const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Jump to nnn + V0 at Bnnn.");
	}
	
	void CPU::opcode_Cxnn( const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Set Vx = rand byte AND kk Cxkk.");
	}
	
	void CPU::opcode_Dxyn( const unsigned int& opcode )
	{
		util::LOG(LOGTYPE::DEBUG, "Opcode: " + std::to_string(opcode) + ", Display n byte sprite starting at mem loc I at (Vx, Vy), set Vf = collision at Dxyn.");
	}
	
	void CPU::opcode_EXxx( const unsigned int& opcode )
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
				util::LOG(LOGTYPE::ERROR, "Unknown opcode for 9xy0: " + std::to_string(opcode));
				break;
			}
		}
	}
	
	void CPU::opcode_FXxx( const unsigned int& opcode )
	{
		switch(opcode & 0x00FF)
		{
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
				util::LOG(LOGTYPE::ERROR, "Unknown opcode for FXxx: " + std::to_string(opcode));
				break;
			}
		}
	}



} // end of chip8 namespace