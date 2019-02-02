#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <SDL2/SDL>

namespace chip8{
	class Controller{
	public:
	protected:
	private:
		const std::array<uint8_t, 16> controller_inputs_ = {
			SDLK_1,		// 1
			SDLK_2,		// 2	
			SDLK_3,		// 3
			SDLK_4,		// C
			SDLK_q,		// 4
			SDLK_w,		// 5
			SDLK_e,		// 6
			SDLK_r,		// D
			SDLK_a,		// 7
			SDLK_s,		// 8
			SDLK_d,		// 9
			SDLK_f,		// E
			SDLK_z,		// A
			SDLK_x,		// 0
			SDLK_c,		// B
			SDLK_v		// F
		}	
	};
}
#endif // CONTROLLER_H