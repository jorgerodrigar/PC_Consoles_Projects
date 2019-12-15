#pragma once
#include <vector>
#include <Logic/Listener.h>
#include <Logic/Emitter.h>

class GameObject;
class RendererThread;

class GameManager: public Listener, public Emitter
{
private:
	RendererThread* _renderThread;

	std::vector<GameObject*> _gameObjects;

	static GameManager* _instance;

	const float _minBound = 32.0f; // PROVISIONAL
	const float _maxBound = 608.0f; // PROVISIONAL

	float _timePerEvent, _iniTimePerEvent;
	float _timeToClose, _iniTimeToClose;
	float _accFactor;
	float _currentTime;
	int _round;
	bool _isEventActive, _gameOver, _isScrolling;
	int _firstSeenDoor;
	int _numOfDollars;
	int _dollarsInserted;
	int _numOfVisibleDoors;

	void reset();

	void activateRandomEvent();
	void setSeenDollars();
	void gameOver();

public:
	GameManager();
	~GameManager();

	static GameManager* GetInstance();
	static void Release();

	void init(RendererThread* rendererThread);
	void update(double deltaTime);
	void render();
	void handleInput();

	bool allDoorsClosed();
	void getGameBounds(float& minBound, float& maxBound);

	virtual void receiveMessage(const Message& message);
};

