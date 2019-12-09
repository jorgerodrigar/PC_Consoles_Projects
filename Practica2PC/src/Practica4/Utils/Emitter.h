#pragma once
#include <list>
#include <Utils/Message.h>

class Listener;
class Emitter {
private:
	static std::list<Listener*> _listeners;

public:
	Emitter();
	~Emitter();
	void addListener(Listener* listener);
	void removeListener(Listener* listener);
	void sendMessage(const Message& message);
};