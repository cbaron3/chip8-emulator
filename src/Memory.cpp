#include <vector>
#include <string>
#include <exception>
#include <climits>
#include <iostream>
#include <functional>
#include <utility>
#include <algorithm>
#include "../include/Memory.h"
#include "../include/Logger.h"

// TODO(Carl Baron: Jan 30th): Implement exceptions
namespace chip8{

  // Default constructor repurposed to initialize mem_ptr and ram values
  Memory::Memory() : mem_ptr_(0){
    ram_.fill(0);
  }

  // Used to validate address.
  void Memory::check_adr_(uint16_t adr) const{
    if(adr > MEM_SPACE - 1){
      throw std::out_of_range("Address undefined in memory space");
    }
  }

  // Internal function to set the memory pointer at an index in ram
  // Protects the class from having a user modify the mem_ptr without reason
  void Memory::set_memory_pointer_(uint16_t mem_ptr){
    // Validate address range
    check_adr_(mem_ptr);
    mem_ptr_ = mem_ptr;
  }

  // Function to get mem pointer location
  uint16_t Memory::get_memory_pointer() const {
    return mem_ptr_;
  }

  // Read a single byte starting at index location 'adr'
  uint8_t Memory::read_byte(uint16_t adr) const {
    // Validate address range
    check_adr_(adr);

    uint8_t data = ram_[adr];
    // Debug message
    util::LOG(LOGTYPE::DEBUG, "Read " + std::to_string(data) + " from address " + std::to_string(adr));

    // Return byte
    return data;
  }

// Read a two bytes, returned as a short, starting at index location 'adr'
  uint16_t Memory::read_short(uint16_t adr) const {
    // Validate address range
    check_adr_(adr);

    // High byte is the one with the smaller address value (closer to 0)
    uint16_t data = ( (ram_[adr] << 8)) | (ram_[adr+1]);

    util::LOG(LOGTYPE::DEBUG, "Read " + std::to_string(data) + " from address " + std::to_string(adr));

    return data;
  }

  // Read 'count' amount of bytes, returned as a vector of uint8_t, starting at index location adr
  std::vector<uint8_t> Memory::read_bulk(uint16_t adr, uint16_t count) const {
    // Validate address range. Need to check start address and end address
    // end address is start address + count
    check_adr_(adr);
    check_adr_(adr+count);

    // Store 'count' amount of bytes for return starting at 'adr' in vector v
    std::vector<uint8_t> data_list;
    for(auto i = adr; i < count; ++i){
      uint8_t data = this->read_byte(adr);
      data_list.push_back(data);
    }

    return data_list;
  }

  // Store a single byte at the next available index location
  void Memory::store_byte(uint8_t data){
    // Validate address range so we can store a byte in RAM
    check_adr_(mem_ptr_+1);

    util::LOG(LOGTYPE::DEBUG, "Storing " + std::to_string(data) + " from address " + std::to_string(mem_ptr_));

    ram_[mem_ptr_++] = data;
    return;
  }

  // Store 'n' bytes, starting at the next available index location
  // 'n' is the size of the input vector
  void Memory::store_bulk(uint16_t adr, std::vector<uint8_t> data){
    // Validate address range. Need to check start address and end address
    // end address is start address + vector size
    check_adr_(adr);
    check_adr_(adr+data.size());

    // Set the memory pointer to the start address and iterate through input vector, storing each element in Memory
    this->set_memory_pointer_(adr);
    for(auto it = data.begin(); it != data.end(); ++it){
      this->store_byte(*it);
    }
  }

  void Memory::print() const {
    std::cout << "***** MEMORY START*****" << std::endl;
    for(auto it = ram_.begin(); it != ram_.end(); ++it){
      std::cout << std::hex <<(unsigned int)*it << " ";
    }
    std::cout << "***** MEMORY END*****" << std::endl;
  }

    // Use CLIMITS
  MemoryMap::MemoryMap( void ) : MemoryMap(0, INT_MAX)
  { 
      // do nothing
  }

  MemoryMap::MemoryMap(  unsigned int mem_size, unsigned int start_adr ) : start_adr_(start_adr), end_adr_(start_adr + mem_size)
  { 
      // do nothing
  }

  std::byte MemoryMap::read( const unsigned int& adr ) const
  {
      validate_adr_(adr);

      auto find_result = memory_space.find(adr);

      if (find_result == memory_space.end())
          throw std::out_of_range("Address undefined");
      else
          return find_result->second;
  }

  // Add new val to map at adr. First validate adr. If address is undefined, aka not added yet, insert val at adr.
  // If adr does exist, update val at adr iff update is true
  bool MemoryMap::store( const unsigned int& adr, const std::byte& val, const bool& update )
  {
      validate_adr_(adr);

      auto find_result = memory_space.find(adr);

      if (find_result == memory_space.end())
      {
          memory_space.emplace(adr,val);
      }
      else
      {
        if(update == true)
          {
              memory_space.at(find_result->first) = val;
          }
      }
          
  }

  // Used to validate address.
  void MemoryMap::validate_adr_(const unsigned int& adr) const{
  if(adr > end_adr_)
      throw std::out_of_range("Address greater than maximum memory address");

  if(adr < start_adr_)
      throw std::out_of_range("Address less than minimum memory address");
  }
}
