#pragma once
#include "GameObject.h"
#include <Logic/Emitter.h>

///maneja el input del disparo. Lee la informacion recibida de Input
class Shooter :	public GameObject, public Emitter 
{
private:
	float _deadZone;

public:
	Shooter();
	~Shooter();

	virtual void init();
	virtual void handleInput();

	///se redefine render vacio porque no tiene sprite que pintar y en la clase padre se define que pinte un sprite por defecto
	virtual void render(RendererThread* renderThread);
};
