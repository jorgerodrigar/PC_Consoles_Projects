#if defined(_WIN64) || defined(_WIN32) 

#include "PCPlatform.h"
#include <SDL.h>

bool PCPlatform::_initialized = false;

PCPlatform::PCPlatform() {}

void PCPlatform::init()
{
	if (!_initialized) {
		//Inicialización del sistema 
		SDL_Init(SDL_INIT_EVERYTHING);
		_initialized = true;
	}
}

void PCPlatform::release()
{
	if (_initialized) {		 
		SDL_Quit();
		_initialized = false;
	}
}

bool PCPlatform::tick()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			return true;
		}
		else if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
			//TODO: RESIZE WINDOW TO FULLSCREEN
		}
		return false;
	}
}

#endif
