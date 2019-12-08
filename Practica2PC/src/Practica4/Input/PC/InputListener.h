#pragma once
#include <Utils/Listener.h>
#include <SDL_events.h>
#include <list>

class InputListener : Listener {
private:
	std::list <SDL_Event> _events;
public:
	InputListener();
	~InputListener();
	virtual bool receiveMessage(const Message& message);
	void init();
	std::list <SDL_Event> getEvents();
};

