#pragma once
#include "GameObject.h"
#include <Utils/Listener.h>
#include <Utils/Emitter.h>

class BackgroundDoor : public GameObject, public Emitter, public Listener
{
public:
	BackgroundDoor();
	~BackgroundDoor();

	virtual void init();

	virtual bool receiveMessage(const Message& message);
	int const getCenterX() const;
	int const getCenterY() const;

};

