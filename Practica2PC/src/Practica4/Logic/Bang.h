#pragma once
#include <Logic/GameObject.h>
#include <Logic/Listener.h>
#include <Logic/Emitter.h>

class Bang: public GameObject, public Listener, public Emitter
{
public:
	Bang();
	~Bang();

	virtual void init();
	virtual void update(double deltaTime);

	virtual void reset();

	virtual void receiveMessage(const Message& message);
};

