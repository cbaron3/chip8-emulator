#ifndef CHIP8_CPU_H
#define CHIP8_CPU_H

// Project includes
#include "Memory.h"	// For memory map

// C++ includes
#include <array>	// C++ array
#include <stack>	// C++ stack
#include <memory> 	// Memory for unique ptr

/*!
 *  \addtogroup chip8
 *  @{
 */

//! chip8 code
namespace chip8
{

/** Anonymous namespace for definitions */
namespace
{
	constexpr size_t MEM_SPACE = 0x0FFF;   // Const for denoting size of memory map
	constexpr long  FONT_START = 0x0000;   // Const for denoting start of Chip8 program
	constexpr long  PROG_START = 0x0200;   // Const for denoting start of Chip8 program
	constexpr uint8_t SCRN_WIDTH = 64;
  	constexpr uint8_t SCRN_HEIGHT = 32;
}

/**
 * @brief Chip8 interpreter class. Used to handle all chip8 functionality
 */
class Interpreter
{

  public:
	/**
	 * @brief Factory method for interpreter
	 * 
	 * @param memory Memory map unique ptr required for construction
	 * @return std::unique_ptr<Interpreter> resulting interpreter
	 */
	static std::unique_ptr<Interpreter> make_interpreter(std::unique_ptr<MemoryMap> memory);

	/**
	 * @brief Interpret next instruction
	 */
	void next_instruction(void);

	/**
	 * @brief Delay timer getter
	 * 
	 * @return unsigned int Delay timer value
	 */
	unsigned int delay(void) const { return m_delay_timer; }

	/**
	 * @brief Sound timer getter
	 * 
	 * @return unsigned int Sound timer value
	 */
	unsigned int sound(void) const { return m_sound_timer; }

	/**
	 * @brief Get array representing the screen
	 * 
	 * @return std::array<uint32_t, 64 * 32> Array of pixels that are either on or off 
	 */
	std::array<uint32_t, 64 * 32> screen(void) { return m_pixels; }

	/**
	 * @brief Update the key state based on gui input
	 * 
	 * @param t_keys Array of key states for chip8 controller
	 */
	void sync_keys(std::array<bool, 16> t_keys) { this->m_keys = t_keys; }

	/**
	 * @brief Get exit flag
	 * 
	 * @return true If chip8 needed to exit. Else false.
	 */
	bool exit(void) const { return m_exit_flag; }

	/**
	 * @brief Draw flag getter
	 * 
	 * @details Resets the draw flag after being called and is only set again when
	 * 			draw instruction occurs.
	 * 
	 * @return true If need draw instruction. Else, false.
	 */
	bool draw(void);

  private:
	/** Default constructor */
	Interpreter(void);

	/** Private constructor to enforce unique pointer factory method */
	Interpreter(std::unique_ptr<MemoryMap> memory);

	/** Execute an opcode */
	void execute(const unsigned int &opcode);

	/** Interpreter's memory map to pull instructions from */
	std::unique_ptr<MemoryMap> memory_map;

	/** Flag for exit and draw */
	bool m_exit_flag, m_draw_flag;

	/** Timers, index register, program counter */
	unsigned int m_delay_timer, m_sound_timer, m_index_register, m_program_counter;

	/** Screen */
	std::array<uint32_t, 64 * 32> m_pixels;

	/** Key pressed state */
	std::array<bool, 16> m_keys;

	/** Register values */
	std::array<uint8_t, 16> m_registers;

	/** Subroutine stack */
	unsigned int m_sp;
	std::array<uint16_t, 16> m_stack;
	
	/** CPU OPCODE FUNCTION DEFINITIONS BELOW */
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

/*! @} End of Doxygen Groups*/

#endif // CHIP8_CPU_H