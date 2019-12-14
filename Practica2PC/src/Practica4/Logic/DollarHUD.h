#pragma once
#include "GameObject.h"
#include <Utils/Listener.h>

class DollarHUD : public GameObject, public Listener
{
private:
	bool _selected;
	char _id;

public:
	DollarHUD();
	virtual ~DollarHUD();

	virtual void init();
	virtual void reset();

	void setSelected();
	void setUnSelected();
	void depositMoney();

	void setId(char id);
	char getId();

	virtual void receiveMessage(const Message& message);
};

