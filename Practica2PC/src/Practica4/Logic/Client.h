#pragma once
#include "GameObject.h"
#include <Logic/Listener.h>
#include <Logic/Emitter.h>

class Client : public GameObject, public Emitter, public Listener
{
private:
	char _id = 0;
	char _currentDoorId;

	///hace aparecer al cliente en una puerta
	void openDoor();
	///desactiva el gameObject. Manda un mensaje de GameOver si ha sido disparado o manda un mensaje de DEPOSIT, junto con _currentDoorId para identificar
	///la puerta en la que se encuentra si no lo ha sido
	void closeDoor();
	///activa la animacion de muerto
	void getAShot();

public:
	Client();
	~Client();

	virtual void init();
	virtual void reset();

	///recibe los mensajes necesarios para comunicarse con el resto de la logica del juego
	virtual void receiveMessage(const Message& message);
};

