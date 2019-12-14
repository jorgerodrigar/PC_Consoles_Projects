#pragma once
#include "GameObject.h"
#include <Utils/Listener.h>
#include <Utils/Emitter.h>

class Bandit : public GameObject, public Emitter, public Listener
{
private:
	bool _isAiming;
	char _id = 1;
	char _currentDoorId;

	void randomAiming();
	void openDoor();
	void closeDoor();
	void getAShot();

public:
	Bandit();
	~Bandit();

	virtual void init();

	virtual void receiveMessage(const Message& message);
	virtual void reset();

};

