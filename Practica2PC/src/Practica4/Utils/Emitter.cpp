#include "Emitter.h"
#include <Utils/Listener.h>
#if defined(_WIN64) || defined(_WIN32)

std::list<Listener*> Emitter::_listeners;
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
	//TODO: PREGUNTAR SI EL DEVOLVER TRU SE TIENE QUE HACER AQUI (UNLIKELY)
	for (Listener* listener : _listeners) {
		listener->receiveMessage(message);
	}
}

#endif
