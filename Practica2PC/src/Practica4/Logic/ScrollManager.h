#pragma once
#include <Logic/Emitter.h>
#include <Logic/GameObject.h>

class GameManager;
class InputData;

///maneja el input de scroll y pinta un unico sprite del fondo de la puerta 4 veces de forma que simule un scroll de 3 puertas
class ScrollManager : public GameObject, public Emitter
{
private:
	GameManager* _gm;
	InputData* _inputData;
	bool _scrollingRight, _scrollingLeft;

	SpriteSheet::Rect _originalRect;

	int _numDoors;
	float _vel;
	int _dir;

	float _minBound, _maxBound;

	float* _targetPositions;
	float* _actualPositions;

	///establece las siguientes posiciones en las que se tiene que pintar el sprite
	void setNextTargetPositions();
	///devuelve si una posicion esta fuera de los limites del juego (barras negras)
	bool isOutsideBounds(float x);

public:
	ScrollManager();
	ScrollManager(float vel, int numDoors = 4);
	virtual ~ScrollManager();

	virtual void init();
	///se reestablecen las posiciones iniciales donde pintar el sprite
	virtual void reset();
	///actualiza las posiciones donde se tiene que pintar el sprite y si se ha alcanzado alguna de ellas, para de hacer scroll y envia mensaje de SCROLL_FINISHED
	virtual void update(double deltaTime);
	///pinta el sprite en cada uno de las posiciones
	virtual void render(RendererThread* renderThread);
	///detecta si se ha pulsado alguna de las teclas de scroll y manda scrollear en la direccion correspondiente
	virtual void handleInput();
};

