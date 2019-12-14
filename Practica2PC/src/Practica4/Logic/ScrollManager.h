#pragma once
#include <Utils/Emitter.h>
#include <Logic/GameObject.h>

class GameManager;
class InputData;

class ScrollManager : public GameObject, public Emitter
{
private:
	GameManager* _gm;
	InputData* _inputData;
	bool _scrollingRight, _scrollingLeft;

	Sprite::Rect _originalRect;

	int _numDoors;
	float _vel;
	int _dir;

	float _minBound, _maxBound;

	float* _targetPositions;
	float* _actualPositions;

	void setNextTargetPositions();
	bool isOutsideBounds(float x);

public:
	ScrollManager();
	ScrollManager(float vel, int numDoors = 4);
	virtual ~ScrollManager();

	virtual void init();
	virtual void reset();

	virtual void update(double deltaTime);
	virtual void render(RendererThread* renderThread);
	virtual void handleInput();
};

