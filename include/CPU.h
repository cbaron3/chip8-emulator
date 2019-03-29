#ifndef CHIP8_CPU_H
#define CHIP8_CPU_H

#include <array>
#include <stack>

namespace chip8
{
	class CPU
	{
	public:
		CPU();

		void execute(uint16_t opcode)
		{
			continue;
		}

		bool draw_flag() const
		{
			return draw;
		}

		std::array<std::array<bool, 64>, 32> get_pixel_map() const
		{
			return pixel_map;
		}

		unsigned int get_pc() const
		{
			return prog_counter_;
		}

		void update_keys(std::array<bool, 16> keys)
		{
			keys_ = keys;
		}

	private:

		std::array<uint16, 16> registers_;		    		// Registers

		std::array<bool, 16> keys_;		    			    // Key state

		bool draw;											// Draw flag

		std::array<std::array<bool, 64>, 32> pixel_map_;	// Pixel state

		unsigned int prog_counter_;							// Where in the program we are 

		unsigned int index_register_;						// Used for index register, which modies operand addresses during program run

		std::stack<uint16_t> stack_;						// Used to store returna ddresses when a subroutine is called
	};
}

#endif // CHIP8_CPU_H