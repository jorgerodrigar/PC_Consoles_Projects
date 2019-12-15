#pragma once
#include <list>
#include <Logic/Message.h>

class Listener;

class Emitter {
private:
	std::list<Listener*> _listeners;

public:
	Emitter();
	~Emitter();
	void addListener(Listener* listener);
	void removeListener(Listener* listener);
	void sendMessage(const Message& message);
};