#if defined(_WIN64) || defined(_WIN32) 

#include "PCRenderer.h"
#include <SDL.h>
#include <iostream>

SDL_Window* PCRenderer::_window = nullptr;
SDL_Renderer* PCRenderer::_renderer = nullptr;
int PCRenderer::WIN_WIDTH = 1280;
int PCRenderer::WIN_HEIGHT = 720;
int PCRenderer::windowType = SDL_WINDOW_SHOWN;

bool PCRenderer::_initialized = false;

PCRenderer::PCRenderer() {}

void PCRenderer::init()
{
	if (!_initialized) {
		int winX, winY;
		winX = winY = SDL_WINDOWPOS_CENTERED;
		//Inicialización del renderer. Creacion de la ventana
		_window = SDL_CreateWindow("Practica 2", winX, winY, WIN_WIDTH, WIN_HEIGHT, windowType);
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
	int* argb = hexToBGR(color);
	SDL_SetRenderDrawColor(_renderer, argb[0], argb[1], argb[2], 0xff);
	SDL_RenderDrawPoint(_renderer, x, y); //Renders on middle of screen.
}

void PCRenderer::present()
{
	SDL_RenderPresent(_renderer);
}

int* PCRenderer::hexToBGR(int hexColor)
{
	int bgr[3];
	bgr[0] = (hexColor >> 16) & 0xFF;  //b
	bgr[1] = (hexColor >> 8) & 0xFF; //g
	bgr[2] = (hexColor) & 0xFF; //r

	return bgr;
}

unsigned const int PCRenderer::getNumBuffers()
{
	return windowType == SDL_WINDOW_FULLSCREEN ? 2 : 1;
}

const int PCRenderer::getWindowWidth()
{
	return WIN_WIDTH;
}

const int PCRenderer::getWindowHeight()
{
	return WIN_HEIGHT;
}

#endif
