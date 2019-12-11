#pragma once
#include <SDL_events.h>
#include <list>

class InputListener {
private:
	std::list <SDL_Event> _events;
public:
	InputListener();
	~InputListener();
	virtual bool receiveEvent(const SDL_Event& event);
	void init();
	std::list <SDL_Event> getEvents();
};

