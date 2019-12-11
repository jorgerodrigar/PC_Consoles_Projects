#if defined(_WIN64) || defined(_WIN32)
#include "InputListener.h"
#include <Platform/Platform.h>

InputListener::InputListener() 
{
}

InputListener::~InputListener()
{
}

bool InputListener::receiveEvent(const SDL_Event& event)
{
	_events.push_back(event);
	return true;
}

void InputListener::init()
{
	Platform::addListener(this);
}

std::list<SDL_Event> InputListener::getEvents()
{
	std::list<SDL_Event> auxList(_events);
	_events.clear();

	return auxList;
}

#endif