#pragma once
#include <Logic/GameObject.h>
#include <Logic/Listener.h>
#include <Logic/Emitter.h>

///se activa cuando es GAME OVER y cuando termina de animarse envia el mensaje de START_ROUND
class Bang: public GameObject, public Listener, public Emitter
{
public:
	Bang();
	~Bang();

	virtual void init();
	virtual void reset();
	virtual void update(double deltaTime);
	virtual void receiveMessage(const Message& message);
};

