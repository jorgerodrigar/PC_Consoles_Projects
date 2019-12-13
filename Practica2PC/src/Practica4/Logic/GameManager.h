#pragma once
#include <vector>
#include <Utils/Listener.h>
#include <Utils/Emitter.h>

class GameObject;
class RendererThread;

class GameManager: public Listener, public Emitter
{
private:
	RendererThread* _renderThread;

	std::vector<GameObject*> _gameObjects;

	static GameManager* _instance;

	float _timePerEvent, _iniTimePerEvent;
	float _timeToClose, _iniTimeToClose;
	float _accFactor;
	float _currentTime;
	int _round;
	bool _isEventActive, _gameOver;

	void reset();

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

	virtual void receiveMessage(const Message& message);
};

