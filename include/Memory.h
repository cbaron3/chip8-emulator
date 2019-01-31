#ifndef CHIP8_MEMORY_H
#define CHIP8_MEMORY_H

#include <array>
#include <vector>
#include "Chip8.h"

namespace chip8{
  class Memory{
  public:

    // Default constructor repurposed to initialize mem_ptr and ram values
    Memory();

    // Read a single byte starting at index location 'adr'
    uint8_t read_byte(uint16_t adr);

    // Read a two bytes, returned as a short, starting at index location 'adr'
    uint16_t read_short(uint16_t adr);

    // Read 'count' amount of bytes, returned as a vector of uint8_t, starting at index location adr
    std::vector<uint8_t> read_bulk(uint16_t adr, uint16_t count);

    // Store a single byte at the next available index location
    void store_byte(uint8_t data);

    // Store 'n' bytes, starting at the next available index location
    // 'n' is the size of the input vector
    void store_bulk(uint16_t adr, std::vector<uint8_t> data);

    // Function to get mem pointer location
    uint16_t get_memory_pointer();

  protected:
  private:
    // Internal function used to validate addresses before accessing ram
    void check_adr_(uint16_t adr);

    // Internal function to set the memory pointer at an index in ram
    // Protects the class from having a user modify the mem_ptr without reason
    void set_memory_pointer_(uint16_t mem_ptr);

    // Array of bytes to defined the memory map
    std::array<uint8_t, MEM_SPACE> ram_;  

    // Used to keep track of where we are in the memory map
    uint16_t mem_ptr_;                    
  };
}

#endif // CHIP8_MEMORY_H