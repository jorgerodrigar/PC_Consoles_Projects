#pragma once
#include "GameObject.h"
#include <Logic/Listener.h>
#include <Logic/Emitter.h>

class DollarHUD : public GameObject, public Listener, public Emitter
{
private:
	bool _selected, _moneyInserted, _lateUnselect, _messageSent;
	char _id;

	///establece el frame de seleccionado, haya dolar depositado o no
	void setSelected();
	///establece el frame de no seleccionado, haya dolar depositado o no
	void setUnSelected();
	///establece la animacion de dinero depositado
	void depositMoney();

public:
	DollarHUD();
	virtual ~DollarHUD();

	virtual void init();
	virtual void reset();
	///comprueba si se ha insertado dinero y manda mensaje al GameManager (solo se envia una vez por ronda)
	virtual void update(double deltaTime);

	char const getId() const;
	void setId(char id);

	///recibe mensajes de si ha sido seleccionado o si han depositado dinero en el
	virtual void receiveMessage(const Message& message);
};

