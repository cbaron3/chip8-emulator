#include <vector>
#include <string>
#include <exception>
#include <climits>
#include <iostream>
#include <functional>
#include <memory>
#include <utility>
#include <algorithm>
#include "../include/Memory.h"
#include "../include/Logger.h"

namespace chip8
{


    // Use CLIMITS
  MemoryMap::MemoryMap( void ) : MemoryMap(INT_MAX, 0)
  { 
      // do nothing
  }

  MemoryMap::MemoryMap( const unsigned int& mem_size, const unsigned int& start_adr ) : start_adr_(start_adr), end_adr_(start_adr + mem_size)
  { 
      // do nothing
  }

  std::unique_ptr<MemoryMap> MemoryMap::makeMemoryMap( const unsigned int& mem_size, const unsigned int& start_adr )
  {
      return make_unique<MemoryMap>(mem_size, start_adr); 
  }

  std::byte MemoryMap::read( const unsigned int& adr ) const
  {
      validate_adr_(adr);

      auto find_result = memory_space.find(adr);

      if (find_result == memory_space.end())
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
  bool MemoryMap::store( const unsigned int& adr, const std::byte& val, const bool& update )
  {
      validate_adr_(adr);

      auto find_result = memory_space.find(adr);

      if (find_result == memory_space.end())
      {
        memory_space.emplace(adr,val);
        return true;
      }
      else
      {
        if(update == true)
        {
          memory_space.at(find_result->first) = val;
          return true;
        }
        else
        {
          return false;
        }
      }
          
  }

  // Used to validate address.
  void MemoryMap::validate_adr_(const unsigned int& adr) const{
  // TODO: (Carl Baron: Mar 29th): Reduce string recreation
  std::string adr_string = " Start: " + std::to_string(start_adr_) + " Requested: " + std::to_string(adr) + " End: " + std::to_string(end_adr_);

  if(adr > end_adr_)
      throw std::out_of_range("Address greater than maximum memory address." + adr_string);

  if(adr < start_adr_)
      throw std::out_of_range("Address less than minimum memory address." + adr_string);
  }

  std::ostream& operator<<(std::ostream& os, const MemoryMap& dt)
  {
      std::vector<unsigned int> map_keys;

      map_keys.reserve (dt.memory_space.size());
      for (auto& it : dt.memory_space) {
          map_keys.push_back(it.first);
      }

      std::sort (map_keys.begin(), map_keys.end());

      os << "***** MEMORY START*****" << std::endl;
      for (auto& it : map_keys) {
        os << std::hex << (int) dt.memory_space.at(it) << ' ';
      }
      os << "***** MEMORY END*****" << std::endl;
      return os;
  }
}
