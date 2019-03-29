#ifndef EMULATOR_H
#define EMULATOR_H  

#include <string>
#include <stack>

#include "Memory.h"
#include "Logger.h"

namespace chip8{
  // This acts as the 'cpu' for the emulator
  class Emulator{
  public:
    void load_fonts();
    void load_rom(std::string fp);
    void print_memory();

    void execute_opcode(uint16_t opcode);
    uint16_t fetch_opcode();

  protected:
  private:
    MemoryMap memory;

    std::stack<uint16_t> stack_;
	  uint16_t prog_counter_ = PROG_START;
  };
}

#endif // EMULATOR_H
