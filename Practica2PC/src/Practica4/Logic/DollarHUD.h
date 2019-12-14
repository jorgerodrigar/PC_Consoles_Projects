#pragma once
#include "GameObject.h"
#include <Utils/Listener.h>
#include <Utils/Emitter.h>

class DollarHUD : public GameObject, public Listener, public Emitter
{
private:
	bool _selected, _moneyInserted;
	char _id;

public:
	DollarHUD();
	virtual ~DollarHUD();

	virtual void init();
	virtual void update(double deltaTime);
	virtual void reset();

	void setSelected();
	void setUnSelected();
	void depositMoney();

	void setId(char id);
	char getId();

	virtual void receiveMessage(const Message& message);
};

