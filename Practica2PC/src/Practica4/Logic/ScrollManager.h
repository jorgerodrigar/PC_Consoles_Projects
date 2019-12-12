#pragma once
#include <Utils/Emitter.h>
#include <Input/InputData.h>

class ScrollManager : public Emitter
{
private:
	InputData _inputData;
	bool _scrollingRight, _scrollingLeft;
	float _vel;

public:
	ScrollManager();
	ScrollManager(float vel);
	virtual ~ScrollManager();

	void update();
};

