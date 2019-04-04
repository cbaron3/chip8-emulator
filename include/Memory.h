#ifndef CHIP8_MEMORY_H
#define CHIP8_MEMORY_H

#include <cstddef>          // Using for C++17 std::byte
#include <unordered_map>    // Unordered map to represent memory space
#include <memory>           // Memory for unique ptr

/**
 * @brief Chip8 components
 *
 * \ingroup chip8
 */
namespace chip8{
    /**
     * @brief      Class for basic c++17 byte memory map.
     */
    class MemoryMap
    {

    private:

        /**
         * @brief      Constructs the object. Private so user has to call factory method
         */
        MemoryMap( void ); 

        /**
         * @brief      Constructs the object. Valid addresses are in range [start address, start_address + mem_size]
         *
         * @param[in]  mem_size   The memory size
         * @param[in]  start_adr  The start address
         */
        MemoryMap( const unsigned int& mem_size, const unsigned int& start_adr = 0 );

        /**
         * @brief      Validate that address is within range. Throw exception if not
         *
         * @param[in]  adr   The address to be validated
         */
        void validate_adr_(const unsigned int& adr) const;

    public:
        /**
         * @brief      Makes a memory map. Valid addresses are in range [start address, start_address + mem_size]
         *
         * @param[in]  mem_size   The memory size
         * @param[in]  start_adr  The start address
         *
         * @return     Unique MemoryMap pointer
         */
        static std::unique_ptr<MemoryMap> makeMemoryMap( const unsigned int& mem_size, const unsigned int& start_adr = 0);

        /**
         * @brief      Store a byte value in
         *
         * @param[in]  val     The value to store in the memory map
         * @param[in]  adr     The address in the memory map where you want to store the value
         * @param[in]  update  Update variable. If true, address adr value will be updated with val if adr exists. If false, address defined exception
         *
         * @return     True if value was added to memory map. Else, false
         */
        bool store( const std::byte& val, const unsigned int& adr, const bool& update = false );

        /**
         * @brief      Read a byte value from memory map
         *
         * @param[in]  adr   The address of the value you want to read
         *
         * @return     Value of byte at address location
         */
        std::byte read( const unsigned int& adr ) const; 

        /**
         * @brief      Overloaded ostream operator to print memory map contents as bytes in hex
         */
        friend std::ostream& operator<<(std::ostream& os, const MemoryMap& dt);
        
        /**
         * @brief      Destroys the object.
         */
        ~MemoryMap( void ) = default;

    private:
        // TODO: Doesn't really need to be a map because we always know the index we are accessing
        std::unordered_map<unsigned int, std::byte> memory_space; /** Unordered map of std::bytes with unsigned int as key to represent memory map */

        int start_adr_; /** Valid start address for memory _map */

        int end_adr_; /** Valid end address for memory_map */
        
    };

} // End of namespace chip8

#endif // CHIP8_MEMORY_H

