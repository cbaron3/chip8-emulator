#ifndef CHIP8_CPU_H
#define CHIP8_CPU_H

#include <array>
#include <stack>
#include <iostream>
#include "Chip8.h"
#include <memory>           // Memory for unique ptr

#include "Memory.h"

namespace chip8
{
	class Interpreter
	{

	public:
		// Factory method
		static std::unique_ptr<Interpreter> make_interpreter( std::unique_ptr<MemoryMap> memory );

		// Interpreter next instruction
		void next_instruction( void );

		/* Shouldn't need setters cause we should have the outside scope modify the interpreter scope */

		// Delay timer get
		unsigned int delay( void ) const { return m_delay_timer; }

		// Sound timer get
		unsigned int sound( void ) const { return m_sound_timer; } 

		// Program counter get
		unsigned int pc( void ) const { return m_program_counter; }

		// Index register get
		unsigned int ir( void ) const { return m_index_register; }

		// Screen pixel get
		std::array<uint32_t, 2048> screen( void ) { return pixels; }

		// Screen pixel print
		void debug_screen( void )
		{
			for(int i = 0; i <2048; ++i)
			{
			      		
			      std::cout << pixels[i] << " ";

			      if(i%64 == 0)
			      {
			      	std::cout << std::endl;
			      }
			      
			  }
		}
		

		// Screen pixel get
		std::array<uint8_t, 16> register_map( void ) { return registers; }

		// Set keys externally
		void sync_keys(std::array<bool, 16> t_keys){ this->keys = t_keys; /* debug_keys(); */ }

		// Exit program flag
		bool exit( void ) const { return m_exit_flag; }

		// Check if we need to update the screen
		bool draw( void ) 
		{ 
			bool temp_flag = m_draw_flag;
			m_draw_flag = false;
			return temp_flag; 
		}

		// TODO: ostream operator for printing class contents

	private:
		// Default constructor
		Interpreter( void );

		// Private constructor to enforce unique pointer factory method
		Interpreter(std::unique_ptr<MemoryMap> memory);

		void debug_keys( void )
		{
			for(int i = 0; i < 16; ++i)
			{
				std::cout << chip8::sdl_key_strings[i] << " " << std::boolalpha << keys[i] << std::endl;
			}
		}

		// Execute an opcode
		void execute( const unsigned int& opcode );

		// Interpreter's memory map to pull instructions from
		std::unique_ptr<MemoryMap> memory_map; 

		// Flag for exit
		bool m_exit_flag, m_draw_flag;

		// Timers, index register, program counter
		unsigned int m_delay_timer, m_sound_timer, m_index_register, m_program_counter;

		// Screen
		std::array<uint32_t, 64*32> pixels;

		// Key pressed state
		std::array<bool, 16> keys;

		// Register values
		std::array<uint8_t, 16> registers;

		// Subroutine stack
		std::array<uint16_t, 16> subroutine_stack;
		uint16_t sp;

		/* CPU OPCODE FUNCTION DEFINITIONS BELOW */
		typedef void(*OpcodeTable)( Interpreter* cpu, const unsigned int& opcode );

		OpcodeTable opcodes[16];

		static void opcode_0xxx( Interpreter* cpu, const unsigned int& opcode );

		static void opcode_1nnn( Interpreter* cpu, const unsigned int& opcode );
		
		static void opcode_2nnn( Interpreter* cpu, const unsigned int& opcode );
		
		static void opcode_3xnn( Interpreter* cpu, const unsigned int& opcode );
		
		static void opcode_4xnn( Interpreter* cpu, const unsigned int& opcode );
		
		static void opcode_5xy0( Interpreter* cpu, const unsigned int& opcode );
		
		static void opcode_6xnn( Interpreter* cpu, const unsigned int& opcode );
		
		static void opcode_7xnn( Interpreter* cpu, const unsigned int& opcode );
		
		static void opcode_8XYx( Interpreter* cpu, const unsigned int& opcode );
		
		static void opcode_9xy0( Interpreter* cpu, const unsigned int& opcode );
		
		static void opcode_Annn( Interpreter* cpu, const unsigned int& opcode );
		
		static void opcode_Bxnn( Interpreter* cpu, const unsigned int& opcode );
		
		static void opcode_Cxnn( Interpreter* cpu, const unsigned int& opcode );
		
		static void opcode_Dxyn( Interpreter* cpu, const unsigned int& opcode );
		
		static void opcode_EXxx( Interpreter* cpu, const unsigned int& opcode );
		
		static void opcode_FXxx( Interpreter* cpu, const unsigned int& opcode );
	};
}

#endif // CHIP8_CPU_H