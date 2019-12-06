#if defined(_WIN64) || defined(_WIN32)

#include "PCInput.h"

InputListener PCInput::_inputListener;

void PCInput::init()
{
	_inputListener.init();
}

void PCInput::release()
{
}

void PCInput::tick()
{
}

InputData PCInput::getUserInput()
{
	InputData inputData = InputData();
	std::list<SDL_Event> eventList = _inputListener.getEvents();
	for (SDL_Event event : eventList) {
		switch (event.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			inputData.buttonsInfo.CIRCLE = 888;
			break;
		default:
			break;
		}
	}

	return inputData;
}

PCInput::PCInput()
{
}

PCInput::~PCInput()
{
}

#endif
