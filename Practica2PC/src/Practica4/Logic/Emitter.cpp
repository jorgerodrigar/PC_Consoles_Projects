#include "Emitter.h"
#include <Logic/Listener.h>

Emitter::Emitter()
{
}

Emitter::~Emitter()
{
}

void Emitter::addListener(Listener * listener)
{
	_listeners.push_back(listener);
}

void Emitter::removeListener(Listener * listener)
{
	_listeners.remove(listener);
}

void Emitter::sendMessage(const Message& message)
{
	for (Listener* listener : _listeners) {
		listener->receiveMessage(message);
	}
}