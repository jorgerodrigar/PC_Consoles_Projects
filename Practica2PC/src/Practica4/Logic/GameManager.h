#pragma once
#include <vector>
#include <Utils/Listener.h>
#include <Utils/Emitter.h>

class RendererThread;
class DoorEvent;
class DollarHUD;
class Shooter;
class ScrollManager;

class GameManager: public Listener, public Emitter
{
private:
	RendererThread* _renderThread;

	std::vector<DollarHUD> _dollars;
	std::vector<DoorEvent*> _doors;
	Shooter* _shooter;

	ScrollManager* _scrollManager;
	float targetPositions[4]; // PROVISIONAL
	int scrollDir;
	float scrollVel;

	static GameManager* _instance;

	/*PROVISIONAL*/
	bool kk;
	int frame;
	/*PROVISIONAL*/

	bool isOutsideBounds(float x);

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

