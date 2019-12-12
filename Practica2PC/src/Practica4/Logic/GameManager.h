#pragma once
#include <vector>
#include <Utils/Listener.h>
#include <Utils/Emitter.h>
#include <Logic/Sprite.h>

class RendererThread;
class DoorEvent;
class DollarHUD;
class Shooter;
class ScrollManager;

class GameManager: public Listener, public Emitter
{
private:
	struct ScreenDoor {
		int currentId;
		int currentDollar;
	};

	RendererThread* _renderThread;

	std::vector<DollarHUD> _dollars;
	std::vector<DoorEvent*> _doors; //TODO hacer que no sean punteros
	std::vector<ScreenDoor> _screenDoors;
	Shooter* _shooter;

	Sprite _bangSprite;

	ScrollManager* _scrollManager;
	float targetPositions[4]; // PROVISIONAL
	int scrollDir;
	float scrollVel;

	static GameManager* _instance;

	float _timePerEvent;
	float _accFactor;
	float _currentTime;
	int _round;
	bool _isEventActive, _gameOver;

	bool isOutsideBounds(float x);

	void reset();

	void addDoor(int id);
	void addDollar(int id);
	void activateRandomEvent();
	void gameOver();

public:
	GameManager();
	~GameManager();

	static GameManager* getInstance();
	static void release();

	void init(RendererThread* rendererThread);
	void update(double deltaTime);
	void render();
	void handleInput();

	virtual bool receiveMessage(const Message& message);
};

