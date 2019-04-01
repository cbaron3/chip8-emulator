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

	private:
		CPU();

		/* CPU OPCODE FUNCTION DEFINITIONS BELOW */
		typedef void(*OpcodeTable)( const unsigned int& opcode );

		OpcodeTable opcodes[16];

		static void opcode_0xxx( const unsigned int& opcode );

		static void opcode_1nnn( const unsigned int& opcode );
		
		static void opcode_2nnn( const unsigned int& opcode );
		
		static void opcode_3xnn( const unsigned int& opcode );
		
		static void opcode_4xnn( const unsigned int& opcode );
		
		static void opcode_5xy0( const unsigned int& opcode );
		
		static void opcode_6xnn( const unsigned int& opcode );
		
		static void opcode_7xnn( const unsigned int& opcode );
		
		static void opcode_8XYx( const unsigned int& opcode );
		
		static void opcode_9xy0( const unsigned int& opcode );
		
		static void opcode_Annn( const unsigned int& opcode );
		
		static void opcode_Bxnn( const unsigned int& opcode );
		
		static void opcode_Cxnn( const unsigned int& opcode );
		
		static void opcode_Dxyn( const unsigned int& opcode );
		
		static void opcode_EXxx( const unsigned int& opcode );
		
		static void opcode_FXxx( const unsigned int& opcode );
	};
}

#endif // CHIP8_CPU_H