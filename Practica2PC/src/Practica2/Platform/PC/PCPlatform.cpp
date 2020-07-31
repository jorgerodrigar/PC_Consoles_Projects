#if defined(_WIN64) || defined(_WIN32) 

#include "PCPlatform.h"
#include <SDL.h>

bool PCPlatform::_initialized = false;

PCPlatform::PCPlatform() {}

void PCPlatform::init()
{
	if (!_initialized) {
		//Inicializaci�n del sistema 
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
		if (event.type == SDL_QUIT ||
			event.type == SDL_KEYDOWN ||
			event.type == SDL_MOUSEBUTTONDOWN) {
			return true;
		}
		return false;
	}
}

#endif
