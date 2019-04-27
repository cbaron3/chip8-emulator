#ifndef CHIP8_CPU_H
#define CHIP8_CPU_H

#include <array>
#include <stack>
#include <iostream>
#include "Chip8.h"
#include <memory> // Memory for unique ptr

#include "Memory.h"

namespace chip8
{
class Interpreter
{

  public:
	// Factory method
	static std::unique_ptr<Interpreter> make_interpreter(std::unique_ptr<MemoryMap> memory);

	// Interpreter next instruction
	void next_instruction(void);

	// Delay timer get
	unsigned int delay(void) const { return m_delay_timer; }

	// Sound timer get
	unsigned int sound(void) const { return m_sound_timer; }

	// Screen pixel get. uint32_t so it can be used
	std::array<uint32_t, 64 * 32> screen(void) { return m_pixels; }

	// Set keys externally
	void sync_keys(std::array<bool, 16> t_keys) { this->m_keys = t_keys; }

	// Exit program flag
	bool exit(void) const { return m_exit_flag; }

	// Check if we need to update the screen. Clear flag afterwards
	bool draw(void)
	{
		bool temp_flag = m_draw_flag;
		m_draw_flag = false;
		return temp_flag;
	}

  private:
	// Default constructor
	Interpreter(void);

	// Private constructor to enforce unique pointer factory method
	Interpreter(std::unique_ptr<MemoryMap> memory);

	// Execute an opcode
	void execute(const unsigned int &opcode);

	// Interpreter's memory map to pull instructions from
	std::unique_ptr<MemoryMap> memory_map;

	// Flag for exit
	bool m_exit_flag, m_draw_flag;

	// Timers, index register, program counter
	unsigned int m_delay_timer, m_sound_timer, m_index_register, m_program_counter;

	// Screen
	std::array<uint32_t, 64 * 32> m_pixels;

	// Key pressed state
	std::array<bool, 16> m_keys;

	// Register values
	std::array<uint8_t, 16> m_registers;

	// Subroutine stack
	unsigned int m_sp;
	std::array<uint16_t, 16> m_stack;
	

	/* CPU OPCODE FUNCTION DEFINITIONS BELOW */
	typedef void (*OpcodeTable)(Interpreter *cpu, const unsigned int &opcode);

	OpcodeTable opcodes[16];

	static void opcode_0xxx(Interpreter *cpu, const unsigned int &opcode);

	static void opcode_1nnn(Interpreter *cpu, const unsigned int &opcode);

	static void opcode_2nnn(Interpreter *cpu, const unsigned int &opcode);

	static void opcode_3xnn(Interpreter *cpu, const unsigned int &opcode);

	static void opcode_4xnn(Interpreter *cpu, const unsigned int &opcode);

	static void opcode_5xy0(Interpreter *cpu, const unsigned int &opcode);

	static void opcode_6xnn(Interpreter *cpu, const unsigned int &opcode);

	static void opcode_7xnn(Interpreter *cpu, const unsigned int &opcode);

	static void opcode_8XYx(Interpreter *cpu, const unsigned int &opcode);

	static void opcode_9xy0(Interpreter *cpu, const unsigned int &opcode);

	static void opcode_Annn(Interpreter *cpu, const unsigned int &opcode);

	static void opcode_Bxnn(Interpreter *cpu, const unsigned int &opcode);

	static void opcode_Cxnn(Interpreter *cpu, const unsigned int &opcode);

	static void opcode_Dxyn(Interpreter *cpu, const unsigned int &opcode);

	static void opcode_EXxx(Interpreter *cpu, const unsigned int &opcode);

	static void opcode_FXxx(Interpreter *cpu, const unsigned int &opcode);
};
} // namespace chip8

#endif // CHIP8_CPU_H