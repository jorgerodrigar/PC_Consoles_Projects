#include "InputListener.h"
#include <iostream>
#include <Platform/Platform.h>
#if defined(_WIN64) || defined(_WIN32)

InputListener::InputListener() 
{
}

InputListener::~InputListener()
{
}

void InputListener::receiveMessage(const Message & message)
{
	switch (message.type)
	{
	case MessageType::TEST:
		std::cout << static_cast<const TestMessage*>(&message)->test << std::endl;
		break;
	case MessageType::INPUT_EVENT:
		SDL_Event event = static_cast<const InputEventMessage*>(&message)->event;
		_events.push_back(event);
		break;
	default:
		break;
	}
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