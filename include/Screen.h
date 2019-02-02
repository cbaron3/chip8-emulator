#ifndef SCREEN_H
#define SCREEN_H

#include <array>
#include "Chip8.h"

namespace chip8{
	class Screen{
	public:
		void clear();
	protected:
	private:
		std::array<std::array<bool, SCRN_WIDTH>, SCRN_HEIGHT> screen_state_;
	};
}

#endif // SCREEN_H