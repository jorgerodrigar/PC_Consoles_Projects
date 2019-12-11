#pragma once
#include "GameObject.h"
class DollarHUD : public GameObject
{
private:
	bool _selected;

public:
	DollarHUD();
	virtual ~DollarHUD();

	virtual void init();

	void switchSelected();
	void depositMoney();
};

