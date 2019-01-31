// Main file
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "../include/Memory.h"
#include "../include/Logger.h"


std::vector<uint8_t> load_rom(std::string file_name);

int main(int argc, char **argv){
	std::cout << "Running main function..." << std::endl;
	util::Logger::getInstance()->set_max_log_level(LOGTYPE::DEBUG);

	switch (argc) {
	    case 1:
	    {
	      std::cout << "No CL arguments supplied" << std::endl;
	      break;
	    }
	    case 2:
	    {
	   	  std::string s(argv[1]);
	      util::LOG(LOGTYPE::DEBUG, s);
	      break;
	    }
	    default:
	    {
	      std::cout << "Invalid CL arguments supplied" << std::endl;
	      break;
	  }
	}

	std::vector<uint8_t> rom = load_rom("../roms/full_games/PONG");

	for(auto it = rom.begin(); it != rom.end(); ++it){
		std::cout << (unsigned int)*it << " ";
	}

	std::cout << "Ending main function..." << std::endl;
	return 0;
}

std::vector<uint8_t> load_rom(std::string file_name){
	std::vector<uint8_t> rom;

	std::ifstream f_rom(file_name);
	
	if(f_rom.is_open()){
		unsigned char c = f_rom.get();

		while(f_rom.good()){
			uint8_t data = (uint8_t) c;
			rom.push_back(data);
			c = f_rom.get();
		}
	}
	else{
		//util::LOG(LOGTYPE::ERROR, "File: " + file_name + " failed to open.");
	}

	f_rom.close();

	return rom;
}