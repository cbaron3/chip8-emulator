#ifndef EMULATOR_H
#define EMULATOR_H  

#include <string>
#include <stack>
#include <memory>

#include "Memory.h"
#include "Logger.h"
#include "CPU.h"

#include "chip8.h"

namespace chip8{
  // This acts as the 'cpu' for the emulator
  class Emulator{
  public:

    Emulator() = delete;
    Emulator(std::unique_ptr<MemoryMap> memory, std::unique_ptr<CPU> cpu);

    void load_rom(std::string fp);
    void print_memory();
    void next_instruction();
  protected:
  private:
    std::unique_ptr<MemoryMap> memory_map;
    std::unique_ptr<CPU> chip8_cpu;

    std::stack<uint16_t> stack_;
	  uint16_t prog_counter_ = PROG_START;
  };
}

#endif // EMULATOR_H
