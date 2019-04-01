#ifndef CHIP8_CPU_H
#define CHIP8_CPU_H

#include <array>
#include <stack>
#include <memory>           // Memory for unique ptr


namespace chip8
{
	class CPU
	{

	public:
		void execute( const unsigned int& opcode);

		static std::unique_ptr<CPU> makeCPU();

		// For testing
		void print_pixels( void ) const;

	private:
		CPU();

	public:
		std::array<std::array<bool, 64>, 32> pixels;
		std::array<unsigned int, 16> registers;
		std::stack<unsigned int> subroutine_stack;
		unsigned int prog_counter;
		bool exit;

	private:
		/* CPU OPCODE FUNCTION DEFINITIONS BELOW */
		typedef void(*OpcodeTable)( CPU* cpu, const unsigned int& opcode );

		OpcodeTable opcodes[16];

		static void opcode_0xxx( CPU* cpu, const unsigned int& opcode );

		static void opcode_1nnn( CPU* cpu, const unsigned int& opcode );
		
		static void opcode_2nnn( CPU* cpu, const unsigned int& opcode );
		
		static void opcode_3xnn( CPU* cpu, const unsigned int& opcode );
		
		static void opcode_4xnn( CPU* cpu, const unsigned int& opcode );
		
		static void opcode_5xy0( CPU* cpu, const unsigned int& opcode );
		
		static void opcode_6xnn( CPU* cpu, const unsigned int& opcode );
		
		static void opcode_7xnn( CPU* cpu, const unsigned int& opcode );
		
		static void opcode_8XYx( CPU* cpu, const unsigned int& opcode );
		
		static void opcode_9xy0( CPU* cpu, const unsigned int& opcode );
		
		static void opcode_Annn( CPU* cpu, const unsigned int& opcode );
		
		static void opcode_Bxnn( CPU* cpu, const unsigned int& opcode );
		
		static void opcode_Cxnn( CPU* cpu, const unsigned int& opcode );
		
		static void opcode_Dxyn( CPU* cpu, const unsigned int& opcode );
		
		static void opcode_EXxx( CPU* cpu, const unsigned int& opcode );
		
		static void opcode_FXxx( CPU* cpu, const unsigned int& opcode );
	};
}

#endif // CHIP8_CPU_H