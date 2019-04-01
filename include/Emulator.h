#ifndef EMULATOR_H
#define EMULATOR_H  

#include <string>
#include <stack>
#include <memory>

#include "Memory.h"
#include "Logger.h"

#include "chip8.h"

namespace chip8{
  // This acts as the 'cpu' for the emulator
  class Emulator{
  public:

    Emulator() = delete;
    Emulator(std::unique_ptr<MemoryMap> memory);

    void load_fonts();
    void load_rom(std::string fp);
    void print_memory();

    void execute_opcode(uint16_t opcode);
    uint16_t fetch_opcode();

  protected:
  private:
    std::unique_ptr<MemoryMap> memory_map;

    std::stack<uint16_t> stack_;
	  uint16_t prog_counter_ = PROG_START;
  };
}

#endif // EMULATOR_H
