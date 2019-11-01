#ifdef _WIN32 

#include "PCPlatform.h"
#include <SDL.h>

PCPlatform::PCPlatform()
{
}

PCPlatform::~PCPlatform()
{
}

void PCPlatform::init()
{
	int winX, winY;
	winX = winY = SDL_WINDOWPOS_CENTERED;
	//Inicialización del sistema y renderer
	SDL_Init(SDL_INIT_EVERYTHING);
	_window = SDL_CreateWindow("Moonace", winX, winY, 800, 600, SDL_WINDOW_SHOWN);
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
	SDL_ShowCursor(0);
}

void PCPlatform::release()
{
}

void PCPlatform::tick()
{
}

#endif
