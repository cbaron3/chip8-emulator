#ifndef CHIP8_MEMORY_H
#define CHIP8_MEMORY_H

#include <array>
#include <vector>
#include <unordered_map>
#include <cstddef>
#include <climits>
#include "Chip8.h"

namespace chip8{
  class Memory{
  public:

    // Default constructor repurposed to initialize mem_ptr and ram values
    Memory();

    // Read a single byte starting at index location 'adr'
    uint8_t read_byte(uint16_t adr) const;

    // Read a two bytes, returned as a short, starting at index location 'adr'
    uint16_t read_short(uint16_t adr) const;

    // Read 'count' amount of bytes, returned as a vector of uint8_t, starting at index location adr
    std::vector<uint8_t> read_bulk(uint16_t adr, uint16_t count) const;

    // Store a single byte at the next available index location
    void store_byte(uint8_t data);

    // Store 'n' bytes, starting at the next available index location
    // 'n' is the size of the input vector
    void store_bulk(uint16_t adr, std::vector<uint8_t> data);

    // Function to get mem pointer location
    uint16_t get_memory_pointer() const;

    // Utility function
    void print() const;

  protected:
  private:
    // Internal function used to validate addresses before accessing ram
    void check_adr_(uint16_t adr) const;

    // Internal function to set the memory pointer at an index in ram
    // Protects the class from having a user modify the mem_ptr without reason
    void set_memory_pointer_(uint16_t mem_ptr);

    // Array of bytes to defined the memory map
    std::array<uint8_t, MEM_SPACE> ram_;  

    // Used to keep track of where we are in the memory map
    uint16_t mem_ptr_;                    
  };
}

namespace chip8
{
    // so user can have addresses and values be whatever
    class MemoryMap
    {
    public:
        // Memory constructors. Map doesnt need size, just cares about it for input validation for user
        MemoryMap( void );     // 
        MemoryMap( unsigned int mem_size, unsigned int start_adr = 0 );

        std::byte read( const unsigned int& adr ) const; 
        bool store( const unsigned int& adr, const std::byte& val, const bool& update = false );  // If already exists, update or not. // Create new key if does not exist. Verify map complexity

        // void set_access_limits( int start_adr = 0, int size = INT_MAX );

        // Destructor
        ~MemoryMap( void ) = default;

    private:
        void validate_adr_(const unsigned int& adr) const;

        int start_adr_;
        int end_adr_;
        std::unordered_map<unsigned int, std::byte> memory_space;    // map of bytes, C++17 for bytes
    };
} // End of namespace chip8

#endif // CHIP8_MEMORY_H

