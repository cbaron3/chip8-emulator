#include <cstring>
#include <array>

#include "../include/Chip8.h"
#include "../include/Logger.h"
#include "../include/Screen.h"

namespace chip8{
	// Responding to clear screen opcode
	void Screen::clear(){
		for(int i =0; i < SCRN_WIDTH; ++i){
			for(int j = 0; j < SCRN_HEIGHT; ++j){
				screen_state_[i][j] = false;
			}
		}
	}
}