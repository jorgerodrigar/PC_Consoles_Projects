#if defined(_WIN64) || defined(_WIN32) 

#include "PCPlatform.h"
#include <Utils/Listener.h>
#include <Utils/Message.h>
#include <SDL.h>

bool PCPlatform::_initialized = false;
std::list<Listener*> PCPlatform::_listeners;
double PCPlatform::_currentTime;

PCPlatform::PCPlatform() {}

void PCPlatform::init()
{
	if (!_initialized) {
		//Inicialización del sistema 
		SDL_Init(SDL_INIT_EVERYTHING);
		_initialized = true;
	}
	_currentTime = SDL_GetTicks();
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
			return false;
		}
		InputEventMessage message = InputEventMessage(MessageType::INPUT_EVENT, event);
		sendMessage(message);
		return true;
	}
}

double PCPlatform::getDeltaTime()
{
	double lastTime = _currentTime;
	_currentTime = SDL_GetTicks();
	return (_currentTime - lastTime)/1000;
}

void PCPlatform::addListener(Listener * listener)
{
	_listeners.push_back(listener);
}

void PCPlatform::removeListener(Listener * listener)
{
	_listeners.remove(listener);
}

void PCPlatform::sendMessage(const Message& message)
{
	for (Listener* listener : _listeners) {
		if(listener->receiveMessage(message)) break;
	}
}

uint32_t * PCPlatform::toCurrentEndian(uint32_t * endian, uint32_t size)
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
