#pragma once
#include <vector>
#include <Logic/Listener.h>
#include <Logic/Emitter.h>

class GameObject;
class RendererThread;

///clase singleton. Es la manejadora de la logica. Tiene la lista de gameObjects. Los comunica entre ellos y realiza el update / render / handleInput de cada uno.
///Contiene el ciclo de juego, controla ganar, perder...
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
	bool _isEventActive, _gameOver, _isScrolling;
	int _firstSeenDoor;
	int _numOfDollars;
	int _dollarsInserted;
	int _numOfVisibleDoors;

	GameManager();
	~GameManager();

	///llama al reset de todos los gameObjects y actualiza la ronda. Si ganas aumenta la ronda, si pierdes reinicia
	void reset();

	///cada _timePerEvent se envia un mensaje a una puerta aleatoria para que se abra
	void activateRandomEvent();
	///establece los dolares seleccionados en el momento (puertas visibles)
	void setSeenDollars();
	///envia un mensaje de clear y desactiva todos los gameObjects
	void gameOver();

public:
	const float MIN_BOUND = 32.0f;
	const float MAX_BOUND = 608.0f;

	static GameManager* const GetInstance();
	static void Release();

	///crea e inicializa todos los gameObjects. Los hace listeners y emitters segun conviene
	void init(RendererThread* rendererThread);
	///llama al update de los gameObjects y activa las puertas pasado cierto tiempo
	void update(double deltaTime);
	///llama al render de todos los gameObjects
	void render();
	///llama al handleInput de todos los gameObjects
	void handleInput();

	///devuelve si todas las puertas estan cerradas (una de las condiciones para poder scrollear)
	bool const allDoorsClosed() const;

	virtual void receiveMessage(const Message& message);
};

