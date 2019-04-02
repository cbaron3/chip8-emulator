#ifndef CHIP8_GENERATE_OPCODES_H
#define CHIP8_GENERATE_OPCODES_H

// Easy yet inefficient way to simulate opcodes for unit testing
// Every opcode has a generate function which does not care about implementation of opcodes just the value
// I choose this method to create readable code
// Looking at this code after writing it, i should have maybe just wrote the 4 standard opcode templates and used typedef'ing to reduce copy paste

namespace chip8
{
	namespace util
	{
		/**
		 * @brief      Generate opcode for clear screen 0x00E0
		 *
		 * @return     Correct opcode for clear screen
		 */
		inline unsigned int clear_scr_call( void ) { return 224; }

		/**
		 * @brief      Generate opcode for return from subroutine 0x00EE
		 *
		 * @return     Correct opcode for subroutine return
		 */
		inline unsigned int ret_subr_call( void ) { return 238; }

		/**
		 * @brief      Generate opcode for setting pc 1nnn
		 *
		 * @param[in]  pc    Value of program counter to set
		 *
		 * @return     Correct opcode for program counter set
		 */
		inline unsigned int set_pc_call( const unsigned int& pc ) { return 0x1000 | pc; }

		/**
		 * @brief      Generate opcode for subroutine call 2nnn
		 *
		 * @param[in]  pc    Value of program counter to add to stack
		 *
		 * @return     Correct opcode for subroutine call
		 */
		inline unsigned int subr_call(const unsigned int& pc) { return 0x2000 | pc; }

		/**
		 * @brief      Generate opcode for skip instruct if Vx == kk 3xkk
		 *
		 * @param[in]  reg   Register value. Must be 1 byte value
		 * @param[in]  kk    Value to compare. Must be max 2 bytes
		 *
		 * @return     Correct opcode for skip instruction if equal call
		 */
		inline unsigned int skip_instr_ifeq_call(const unsigned int& reg, const unsigned int& kk) { return 0x3000 | (reg<<8) | kk; }

		/**
		 * @brief      Generate opcode for skip instruct if Vx != kk 4xkk
		 *
		 * @param[in]  reg   Register value. Must be 1 byte value
		 * @param[in]  kk    Value to compare. Must be max 2 bytes
		 *
		 * @return     Correct opcode for skip instruction if not equal call
		 */
		inline unsigned int skip_instr_ifneq_call(const unsigned int& reg, const unsigned int& kk) { return 0x4000 | (reg<<8) | kk; }

		/**
		 * @brief      Generate opcode for skip instruct if Vx == Vy 5xy0
		 *
		 * @param[in]  regx   Register x. Must be 1 byte value
		 * @param[in]  regy   Register x. Must be 1 byte value
		 *
		 * @return     Correct opcode for skip instruction if equal registers call
		 */
		inline unsigned int skip_instr_ifeq_reg_call(const unsigned int& regx, const unsigned int& regy){ return 0x5000 | (regx<<8) | (regy<<4); }

		/**
		 * @brief      Generate opcode for set Vx = kk 6xkk
		 *
		 * @param[in]  reg   Register value. Must be 1 byte value
		 * @param[in]  kk    Value to set. Must be max 2 bytes
		 *
		 * @return     Correct opcode for set Vx to value call
		 */
		inline unsigned int set_reg_call(const unsigned int& reg, const unsigned int& kk) { return 0x6000 | (reg<<8) | kk; }

		/**
		 * @brief      Generate opcode for add kk to Vx 7xkk
		 *
		 * @param[in]  reg   Register value. Must be 1 byte value
		 * @param[in]  kk    Value to add. Must be max 2 bytes
		 *
		 * @return     Correct opcode for add value to Vx to value
		 */
		inline unsigned int add_to_reg_call(const unsigned int& reg, const unsigned int& kk) { return 0x7000 | (reg<<8) | kk; }

		/**
		 * @brief      Generate opcode for set Vx = Vy 8xy0
		 *
		 * @param[in]  regx   Register x. Must be 1 byte value
		 * @param[in]  regy   Register x. Must be 1 byte value
		 *
		 * @return     Correct opcode for set Vx equal to Vy call
		 */
		inline unsigned int set_reg_equal_call(const unsigned int& regx, const unsigned int& regy){ return 0x8000 | (regx<<8) | (regy<<4); }

		/**
		 * @brief      Generate opcode for set Vx = Vx OR Vy 8xy1
		 *
		 * @param[in]  regx   Register x. Must be 1 byte value
		 * @param[in]  regy   Register x. Must be 1 byte value
		 *
		 * @return     Correct opcode for set Vx |= Vy
		 */
		inline unsigned int or_reg_call(const unsigned int& regx, const unsigned int& regy){ return 0x8001 | (regx<<8) | (regy<<4); }

		/**
		 * @brief      Generate opcode for set Vx = Vx AND Vy 8xy2
		 *
		 * @param[in]  regx   Register x. Must be 1 byte value
		 * @param[in]  regy   Register x. Must be 1 byte value
		 *
		 * @return     Correct opcode for set Vx &= Vy
		 */
		inline unsigned int and_reg_call(const unsigned int& regx, const unsigned int& regy){ return 0x8002 | (regx<<8) | (regy<<4); }

		/**
		 * @brief      Generate opcode for set Vx = Vx XOR Vy 8xy3
		 *
		 * @param[in]  regx   Register x. Must be 1 byte value
		 * @param[in]  regy   Register x. Must be 1 byte value
		 *
		 * @return     Correct opcode for set Vx ^= Vy
		 */
		inline unsigned int xor_reg_call(const unsigned int& regx, const unsigned int& regy){ return 0x8003 | (regx<<8) | (regy<<4); }

		/**
		 * @brief      Generate opcode for Vx += Vx
		 *
		 * @param[in]  regx   Register x. Must be 1 byte value
		 * @param[in]  regy   Register x. Must be 1 byte value
		 *
		 * @return     Correct opcode for Vx += Vy
		 */
		inline unsigned int add_reg_call(const unsigned int& regx, const unsigned int& regy){ return 0x8004 | (regx<<8) | (regy<<4); }

		/**
		 * @brief      Generate opcode for Vx -= Vx
		 *
		 * @param[in]  regx   Register x. Must be 1 byte value
		 * @param[in]  regy   Register x. Must be 1 byte value
		 *
		 * @return     Correct opcode for Vx -= Vy
		 */
		inline unsigned int sub_reg_call(const unsigned int& regx, const unsigned int& regy){ return 0x8005 | (regx<<8) | (regy<<4); }

		/**
		 * @brief      Generate opcode for Vx SHR 1
		 *
		 * @param[in]  regx   Register x. Must be 1 byte value
		 *
		 * @return     Correct opcode for Vx SHR 1
		 */
		inline unsigned int shr_reg_call(const unsigned int& regx){ return 0x8006 | (regx<<8); }

		/**
		 * @brief      Generate opcode for Vx = Vy-Vx
		 *
		 * @param[in]  regx   Register x. Must be 1 byte value
		 * @param[in]  regy   Register x. Must be 1 byte value
		 *
		 * @return     Correct opcode for Vx = Vy-Vx
		 */
		inline unsigned int subn_reg_call(const unsigned int& regx, const unsigned int& regy){ return 0x8007 | (regx<<8) | (regy<<4); }

		/**
		 * @brief      Generate opcode for Vx SHL 1
		 *
		 * @param[in]  regx   Register x. Must be 1 byte value
		 *
		 * @return     Correct opcode for Vx SHL 1
		 */
		inline unsigned int shl_reg_call(const unsigned int& regx){ return 0x800E | (regx<<8); }

		/**
		 * @brief      Generate opcode for instruction skip if Vx != Vy
		 *
		 * @param[in]  regx   Register x. Must be 1 byte value
		 * @param[in]  regy   Register x. Must be 1 byte value
		 *
		 * @return     Correct opcode for instruction skip if Vx != Vy
		 */
		inline unsigned int skip_instr_ifneq_reg_call(const unsigned int& regx, const unsigned int& regy){ return 0x9000 | (regx<<8) | (regy<<4); }


		/**
		 * @brief      Generate opcode for instruction that sets index register to nnn
		 *
		 * @param[in]  nnn   New index register value
		 *
		 * @return     Correct opcode for instruction that sets index register to nnn
		 */
		inline unsigned int set_i_call(const unsigned int& nnn){ return 0xA000 | nnn;}

		/**
		 * @brief      Generate opcode for instruction that jumps pc to V0 + NNN
		 *
		 * @param[in]  nnn   New index register value
		 *
		 * @return     Correct opcode for for instruction that jumps pc to V0 + NNN
		 */
		inline unsigned int jump_pc_call(const unsigned int& nnn){ return 0xB000 | nnn;}

		/**
		 * @brief      Generate opcode for set Vx to random byte AND kk
		 *
		 * @param[in]  reg   Register value. Must be 1 byte value
		 * @param[in]  kk    Value to AND. Must be max 2 bytes
		 *
		 * @return     Correct opcode for set Vx to random byte AND kk
		 */
		inline unsigned int rand_reg_call(const unsigned int& reg, const unsigned int& kk) { return 0xC000 | (reg<<8) | kk; }

		/**
		 * @brief      Generate opcode for displaying byte sprite at x,y
		 *
		 * @param[in]  regx  The regx. Value of reg is x coord
		 * @param[in]  regy  The regy. Value of reg is y coord
		 * @param[in]  n     Sprite size in bytes
		 *
		 * @return     Correct opcode for displaying byte sprite at x,y
		 */
		inline unsigned int display_sprite_call(const unsigned int& regx, const unsigned int& regy, const unsigned int& n){ return 0xD000 | (regx<<8) | (regy<<4) | n; }

		/**
		 * @brief      Generate opcode for if key with Vx value is pressed, skip instruction
		 *
		 * @param[in]  reg   Register x. Must be 1 byte value
		 *
		 * @return     Correct opcode for if key with Vx value is pressed, skip instruction
		 */
		inline unsigned int key_skip_press_call(const unsigned int& reg){ return 0xE09E  | (reg<<8); }

		/**
		 * @brief      Generate opcode for if key with Vx value is not pressed, skip instruction
		 *
		 * @param[in]  reg   Register x. Must be 1 byte value
		 *
		 * @return     Correct opcode for if key with Vx value is not pressed, skip instruction
		 */
		inline unsigned int key_skip_not_press_call(const unsigned int& reg){ return 0xE0A1  | (reg<<8); }

		/**
		 * @brief      Generate opcode for Vx = delay timer
		 *
		 * @param[in]  reg   Register x. Must be 1 byte value
		 *
		 * @return     Correct opcode for Vx = delay timer
		 */
		inline unsigned int vx_eq_delay_call(const unsigned int& reg){ return 0xF007  | (reg<<8); }

		/**
		 * @brief      Generate opcode for waiting for a key press and storing key in Vx
		 *
		 * @param[in]  reg   Register x. Must be 1 byte value
		 *
		 * @return     Correct opcode for waiting for a key press and storing key in Vx
		 */
		inline unsigned int wait_for_key_call(const unsigned int& reg){ return 0xF00A  | (reg<<8); }


		/**
		 * @brief      Generate opcode for delay timer = Vx
		 *
		 * @param[in]  reg   Register x. Must be 1 byte value
		 *
		 * @return     Correct opcode for delay timer = Vx
		 */
		inline unsigned int delay_eq_vx_call(const unsigned int& reg){ return 0xF015  | (reg<<8); }

		/**
		 * @brief      Generate opcode for sound timer = Vx
		 *
		 * @param[in]  reg   Register x. Must be 1 byte value
		 *
		 * @return     Correct opcode for sound timer = Vx
		 */
		inline unsigned int sound_eq_vx_call(const unsigned int& reg){ return 0xF018  | (reg<<8); }

		/**
		 * @brief      Generate opcode for I += Vx
		 *
		 * @param[in]  reg   Register x. Must be 1 byte value
		 *
		 * @return     Correct opcode for I += Vx
		 */
		inline unsigned int index_add_reg_call(const unsigned int& reg){ return 0xF01E  | (reg<<8); }

		/**
		 * @brief      Generate opcode for I = location of sprite for digit Vx
		 *
		 * @param[in]  reg   Register x. Must be 1 byte value
		 *
		 * @return     Correct opcode for I = location of sprite for digit Vx
		 */
		inline unsigned int index_sprite_call(const unsigned int& reg){ return 0xF029  | (reg<<8); }

		/**
		 * @brief      Generate opcode for storing bcd of Vx in [I, I+2]
		 *
		 * @param[in]  reg   Register x. Must be 1 byte value
		 *
		 * @return     Correct opcode for storing bcd of Vx in [I, I+2]
		 */
		inline unsigned int store_bcd_call(const unsigned int& reg){ return 0xF033  | (reg<<8); }

		/**
		 * @brief      Generate opcode for storing registers [V0, Vx] in memory starting at I
		 *
		 * @param[in]  reg   Register x. Must be 1 byte value
		 *
		 * @return     Correct opcode for storing registers [V0, Vx] in memory starting at I
		 */
		inline unsigned int store_regs_mem_call(const unsigned int& reg){ return 0xF055  | (reg<<8); }

		/**
		 * @brief      Generate opcode for reading into registers [V0, Vx] from memory starting at I
		 *
		 * @param[in]  reg   Register x. Must be 1 byte value
		 *
		 * @return     Correct opcode for storing registers [V0, Vx] in memory starting at I
		 */
		inline unsigned int read_regs_mem_call(const unsigned int& reg){ return 0xF055  | (reg<<8); }







	} // end of util namespace
} // end of chip8 namespace

#endif // CHIP8_GENERATE_OPCODES_H