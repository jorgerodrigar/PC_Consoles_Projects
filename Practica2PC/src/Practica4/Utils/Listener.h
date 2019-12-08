#pragma once
#include <Utils/Message.h>

class Listener {
public:
	Listener();
	~Listener();
	virtual bool receiveMessage(const Message& message) = 0;
};