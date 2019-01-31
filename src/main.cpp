// Main file
#include <iostream>
#include "../include/Memory.h"

#include "../include/Logger.h"


int main(){
	std::cout << "Running main function..." << std::endl;

	chip8::Memory memory;
	memory.store_byte(0xF);
	memory.read_byte(1);

	std::cout << "Ending main function..." << std::endl;
	return 0;
}