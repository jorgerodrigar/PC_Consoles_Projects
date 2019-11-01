#if defined(_WIN64) || defined(_WIN32) 

#include "PCRenderer.h"
#include <SDL.h>
#include <iostream>

SDL_Window* PCRenderer::_window = nullptr;
SDL_Renderer* PCRenderer::_renderer = nullptr;

bool PCRenderer::_initialized = false;

PCRenderer::PCRenderer() {}

void PCRenderer::init(const char* winName,  int winWidth, int winHeight)
{
	if (!_initialized) {
		int winX, winY;
		winX = winY = SDL_WINDOWPOS_CENTERED;
		//Inicialización del renderer
		_window = SDL_CreateWindow(winName, winX, winY, winWidth, winHeight, SDL_WINDOW_SHOWN);
		_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);

		_initialized = true;
	}
}

void PCRenderer::release()
{
	if (_initialized) {
		SDL_DestroyRenderer(_renderer);
		_renderer = nullptr;
		SDL_DestroyWindow(_window);
		_window = nullptr;

		_initialized = false;
	}
}

void PCRenderer::clear(int color)
{
	SDL_SetRenderDrawColor(_renderer, color, color, color, 255);
	SDL_RenderClear(_renderer);
}

void PCRenderer::putPixel(int x, int y, int color)
{
	int* argb = hexToARGB(color);
	SDL_SetRenderDrawColor(_renderer, argb[1], argb[2], argb[3], argb[0]);
	SDL_RenderDrawPoint(_renderer, x, y); //Renders on middle of screen.
}

void PCRenderer::present()
{
	SDL_RenderPresent(_renderer);
}

int* PCRenderer::hexToARGB(int hexColor)
{
	int argb[4];
	argb[0] = hexColor & 0xFF;         //a
	argb[1] = (hexColor >> 8) & 0xFF;  //r
	argb[2] = (hexColor >> 16) & 0xFF; //g
	argb[3] = (hexColor >> 24) & 0xFF; //b

	return argb;
}

int PCRenderer::getWindowWidth()
{
	return 800;
}

int PCRenderer::getWindowHeight()
{
	return 600;
}

#endif
