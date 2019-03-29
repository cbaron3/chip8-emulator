#ifndef CHIP8_CPU_H
#define CHIP8_CPU_H

namespace chip8
{
	class CPU
	{
	public:
		CPU();

		void execute_opcode(uint16_t opcode)
		{
			continue;
		}

		unsigned int get_pc() const
		{
			return prog_counter;
		}
		
	private:
		// Pixels will be interesting
		std::array<uint16, 16> registers;
		unsigned int prog_counter;		// Where in the program we are 
		unsigned int index_register;	// Used for index register, which modies operand addresses during program run
		std::stack<uint16_t> stack;		// Used to store returna ddresses when a subroutine is called
	};
}

#endif // CHIP8_CPU_H