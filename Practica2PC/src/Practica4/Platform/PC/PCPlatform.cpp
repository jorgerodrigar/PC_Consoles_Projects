#if defined(_WIN64) || defined(_WIN32) 

#include "PCPlatform.h"
#include <SDL.h>
#include <Input/PC/InputListener.h>

bool PCPlatform::_initialized = false;
std::list<InputListener*> PCPlatform::_listeners;
double PCPlatform::_currentTime;
double PCPlatform::_deltaTime;

PCPlatform::PCPlatform() {}

void PCPlatform::Init()
{
	if (!_initialized) {
		//Inicialización del sistema 
		SDL_Init(SDL_INIT_EVERYTHING);
		_initialized = true;
	}
	_currentTime = SDL_GetTicks();
}

void PCPlatform::Release()
{
	if (_initialized) {
		SDL_Quit();
		_initialized = false;
	}
}

bool PCPlatform::Tick()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) { // salimos al clickar dentro de la pantalla
		if (event.type == SDL_QUIT || event.type == SDL_MOUSEBUTTONDOWN) {
			return false;
		}
		SendEvent(event);
	}

	double lastTime = _currentTime;
	_currentTime = SDL_GetTicks();
	_deltaTime = (_currentTime - lastTime) / 1000;

	return true;
}

double PCPlatform::GetDeltaTime()
{
	return _deltaTime;
}

void PCPlatform::AddListener(InputListener * listener)
{
	_listeners.push_back(listener);
}

void PCPlatform::RemoveListener(InputListener * listener)
{
	_listeners.remove(listener);
}

void PCPlatform::SendEvent(const SDL_Event& event)
{
	for (InputListener* listener : _listeners) {
		if(listener->receiveEvent(event)) break;
	}
}

uint32_t * PCPlatform::ToCurrentEndian(uint32_t * endian, uint32_t size)
{
	if (SDL_BYTEORDER == SDL_LIL_ENDIAN)
		return _toCurrentEndian(endian, size);
	else
		return endian;
}

uint32_t * PCPlatform::_toCurrentEndian(uint32_t * endian, uint32_t size)
{
	for (int j = 0; j < size; j++)
		endian[j] = ((endian[j] >> 24) & 0x000000ff | (endian[j] >> 8) & 0x0000ff00 | (endian[j] << 8) & 0x00ff0000 | (endian[j] << 24) & 0xff000000);

	return endian;
}

#endif
