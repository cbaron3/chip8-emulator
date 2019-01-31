#ifndef CHIP8_H
#define CHIP8_H
// File for keeping holding Chip8 system variables

#include <iostream>
#include <exception>

namespace chip8{
  constexpr size_t MEM_SPACE = 0xFFF;   // Const for denoting size of memory map
  constexpr long  PROG_START = 0x200;   // Const for denoting start of Chip8 program
}

#endif // CHIP8_H