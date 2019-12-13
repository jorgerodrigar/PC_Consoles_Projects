#pragma once
#include <Utils/Emitter.h>
#include <Logic/GameObject.h>
#include <Input/InputData.h>

class ScrollManager : public GameObject, public Emitter
{
private:
	InputData _inputData;
	bool _scrollingRight, _scrollingLeft;
	float _vel;
	int _dir;

	float _targetPositions[4];
	float _actualPositions[4];

	void setNextTargetPositions();
	bool isOutsideBounds(float x);

public:
	ScrollManager();
	ScrollManager(float vel);
	virtual ~ScrollManager();

	virtual void init();

	virtual void update(double deltaTime);
	virtual void render(RendererThread* renderThread);
	virtual void handleInput();
};

