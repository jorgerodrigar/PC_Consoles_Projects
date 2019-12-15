#pragma once
#include <Logic/Message.h>

class Listener {
public:
	Listener();
	~Listener();
	virtual void receiveMessage(const Message& message) = 0;
};