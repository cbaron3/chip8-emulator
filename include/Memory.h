#ifndef CHIP8_MEMORY_H
#define CHIP8_MEMORY_H

#include <array>
#include <vector>
#include <unordered_map>
#include <cstddef>
#include <climits>
#include "Chip8.h"

namespace chip8{
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

        friend std::ostream& operator<<(std::ostream& os, const MemoryMap& dt);

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

