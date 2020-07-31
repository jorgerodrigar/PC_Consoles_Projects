#pragma once

#include <Logic/SpriteSheet.h>

class RendererThread;

class GameObject {
protected:
	SpriteSheet _spriteSheet;
	float _x, _y;
	bool _active;
    int _pendingFrames;
	///reinicia _pendingFrames a numBuffers. Establece el numero de veces que debe ser pintado
	void setDirty();

public:
	GameObject();
	~GameObject();

	///metodo virtual. Inicia las animaciones y el spritesheet. Por defecto, inicia el spritesheet al de Debug
	virtual void init();
	///es llamado al iniciar cada ronda, maneja el reinicio de los atributos
	virtual void reset();
	///si ha de ser pintado, pinta el spritesheet en pantalla y reduce _pendigFrames
	virtual void render(RendererThread* renderThread);
	///actualiza las animaciones del sprite. Si la animacion ha cambiado, llama a setDirty
	virtual void update(double deltaTime);
	///metodo vacio, a redefinir por los hijos. No es abstracto para poder instanciar GameObjects de prueba
	virtual void handleInput() {};

	bool const getActive() const;
	float const getX() const;
	float const getY() const;

	void setActive(bool value);
	void setX(float value);
	void setY(float value);
};

