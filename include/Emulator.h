#ifndef EMULATOR_H
#define EMULATOR_H  

#include <string>   // For file path
#include <stack>    // For 
#include <memory>

#include "chip8.h"

#include "Memory.h"
#include "Logger.h"
#include "CPU.h"



/**
 * @brief Chip8 components
 *
 * \ingroup chip8
 */
namespace chip8{
    /**
     * @brief      Class for basic c++17 byte memory map.
     */
  class Emulator{
  public:
    // Default constructor deleted to enforce passing a memory ptr and cpu ptr for proper construction
    Emulator() = delete;

    /**
     * @brief      Constructs the object.
     *
     * @param[in]  memory  MemoryMap unique ptr object
     * @param[in]  cpu     CPU unique ptr object
     */
    explicit Emulator(std::unique_ptr<MemoryMap> memory, std::unique_ptr<CPU> cpu);


    /**
     * @brief      Loads a rom into the memory for the emulator
     *
     * @param[in]  fp    file path for the rom
     */
    void load_rom(std::string fp);


    /**
     * @brief      For debugging, prints memory. TODO: REMOVE
     */
    void print_memory();


    /**
     * @brief      Executes the next ROM instruction
     */
    void next_instruction();

  protected:
  private:
    std::unique_ptr<MemoryMap> memory_map;  /** Memory map object for storing bytes */
    std::unique_ptr<CPU> chip8_cpu;         /** CPU object for executing opcodes    */

	  uint16_t prog_counter_ = PROG_START;
  };
}

#endif // EMULATOR_H
