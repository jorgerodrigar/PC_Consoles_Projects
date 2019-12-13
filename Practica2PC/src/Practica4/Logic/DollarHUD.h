#pragma once
#include "GameObject.h"
#include <Utils/Listener.h>

class DollarHUD : public GameObject, public Listener
{
private:
	bool _selected;

public:
	DollarHUD();
	virtual ~DollarHUD();

	virtual void init();

	void switchSelected();
	void depositMoney();

	virtual void receiveMessage(const Message& message) {}; //TODO
};

