#pragma once
#include <Logic/Message.h>

class Listener {
public:
	Listener();
	~Listener();
	///metodo virtual. A redefinir por todos los hijos
	virtual void receiveMessage(const Message& message) = 0;
};