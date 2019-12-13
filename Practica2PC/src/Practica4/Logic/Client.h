#pragma once
#include "GameObject.h"
#include <Utils/Listener.h>
#include <Utils/Emitter.h>

class Client : public GameObject, public Emitter, public Listener
{
private:
	char _id = 0;
	char _currentDoorId;

	void openDoor();
	void closeDoor();
	void getAShot();

public:
	Client();
	~Client();

	virtual void init();

	virtual bool receiveMessage(const Message& message);

};

