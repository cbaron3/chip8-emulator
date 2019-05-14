// Project includes
#include "../include/Memory.h"  // For class definition
#include "../include/Logger.h"  // For logging

// C++ includes
#include <vector>     // For vectors for printing
#include <string>     // For strings
#include <exception>  // For exceptions
#include <climits>    // For integer limits
#include <algorithm>  // For sort needed to print unordered map

namespace chip8
{

// Default constructor
MemoryMap::MemoryMap( void ) : MemoryMap(INT_MAX, 0)
{ 
    // do nothing
}

// Overloaded constructor
MemoryMap::MemoryMap( const unsigned int& mem_size, const unsigned int& start_adr ) : start_adr_(start_adr), end_adr_(start_adr + mem_size)
{ 
    // do nothing
}

// Factory method
std::unique_ptr<MemoryMap> MemoryMap::makeMemoryMap( const unsigned int& mem_size, const unsigned int& start_adr )
{
    // Used to dodge problems with private/protected constructors and static functions
    struct MakeUniquePublic : public MemoryMap {
      MakeUniquePublic( const unsigned int& mem_size, const unsigned int& start_adr ) : MemoryMap(mem_size, start_adr) {}
    };
    
    return std::make_unique<MakeUniquePublic>(mem_size, start_adr); 
}

// Read a byte
std::byte MemoryMap::read( const unsigned int& adr ) const
{
    // Validate address
    validate_adr_( adr );

    // See if address exists
    auto find_result = memory_space.find( adr );

    // If address exists, return value. Else, toss an error
    if ( find_result == memory_space.end() )
    {
        std::string adr_string = " Start: " + std::to_string(start_adr_) + " Requested: " + std::to_string(adr) + " End: " + std::to_string(end_adr_);
        throw std::out_of_range("Address undefined." + adr_string);
    }
    else
    {
        return find_result->second;
    }
}

// Add new val to map at adr. First validate adr. If address is undefined, aka not added yet, insert val at adr.
// If adr does exist, update val at adr iff update is true
bool MemoryMap::store( const std::byte& val, const unsigned int& adr, const bool& update )
{
    // Validate address
    validate_adr_(adr);

    // See if address exists
    auto find_result = memory_space.find(adr);

    // If address doesnt exist, add the value to the map at address
    // Else, update the value if update is true and if it isnt true, do nothing
    if (find_result == memory_space.end())
    {
        memory_space.emplace( adr,val );
        return true;
    }
    else
    {
        if( update == true )
        {
            // Update value
            memory_space.at( find_result->first ) = val;
            return true;
        }
        else
        {
            return false;
        }
    }
        
}

// Used to validate address.
void MemoryMap::validate_adr_(const unsigned int& adr) const
{

std::string adr_string = " Start: " + std::to_string(start_adr_) + " Requested: " + std::to_string(adr) + " End: " + std::to_string(end_adr_);

if(adr > end_adr_)
    throw std::out_of_range("Address greater than maximum memory address." + adr_string);

if(adr < start_adr_)
    throw std::out_of_range("Address less than minimum memory address." + adr_string);

}

// Helper overloaded operator for printing
std::ostream& operator<<(std::ostream& os, const MemoryMap& dt)
{
    // Temp vector to store keys in the map so they can be sorted
    // Only need to be in order for printing, which is mainly a debug utility
    // Hence unordered map vs map
    std::vector<unsigned int> map_keys;
    map_keys.reserve ( dt.memory_space.size() );

    // For every key,value pair in map, store key in vector
    for ( auto& it : dt.memory_space ) 
    {
        map_keys.push_back( it.first );
    }

    // Sort keys where the keys represent address
    std::sort (map_keys.begin(), map_keys.end());

    // Print every value in order of sorted keys
    os << "***** MEMORY START*****" << std::endl;
    for ( auto& it : map_keys ) 
    {
      os << std::hex << (int) dt.memory_space.at( it ) << " ";
    }
    os << "***** MEMORY END*****" << std::endl;
    
    return os;
}

} // End of namespace chip8
