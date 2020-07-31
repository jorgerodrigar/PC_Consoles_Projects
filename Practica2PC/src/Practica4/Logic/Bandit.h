#pragma once
#include "GameObject.h"
#include <Logic/Listener.h>
#include <Logic/Emitter.h>

class Bandit : public GameObject, public Emitter, public Listener
{
private:
	bool _isAiming;
	char _id = 1;
	char _currentDoorId;

	///hace un random para elegir si el bandido aparece apuntando al jugador o no
	void randomAiming();
	///hace aparecer al bandido en una puerta
	void openDoor();
	///desactiva el gameObject. Manda un mensaje de GameOver si apunta al jugador y no ha sido disparado o si no esta apuntando y ha sido disparado
	void closeDoor();
	///activa la animacion de muerto
	void getAShot();

public:
	Bandit();
	~Bandit();

	virtual void init();
	virtual void reset();

	///recibe los mensajes necesarios para comunicarse con el resto de la logica del juego
	virtual void receiveMessage(const Message& message);
};

