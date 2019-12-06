#pragma once
#include <list>
#include <Utils/Message.h>

//TODO: PREGUNTAR ESTO QUE ESTA FEO
class Listener;
class Emitter {
private:
	static std::list<Listener*> _listeners;

public:
	Emitter();
	~Emitter();
	static void addListener(Listener* listener);
	static void removeListener(Listener* listener);
	static void sendMessage(const Message& message);
};