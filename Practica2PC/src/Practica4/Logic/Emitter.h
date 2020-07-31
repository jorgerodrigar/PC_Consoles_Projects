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
	///anyade un listener a la lista de listeners
	void addListener(Listener* listener);
	///quita un listener de la lista de listeners
	void removeListener(Listener* listener);
	///manda un mensaje a todos los listeners
	void sendMessage(const Message& message);
};