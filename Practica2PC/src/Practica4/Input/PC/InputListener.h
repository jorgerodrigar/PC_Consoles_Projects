#pragma once
#include <SDL_events.h>
#include <list>

///listener de PCPlatform. Recibe los eventos detectados por PCPlatform
class InputListener {
private:
	std::list <SDL_Event> _events;
public:
	///constructora por defecto
	InputListener();
	///destructora por defecto
	~InputListener();
	///recibe un evento detectado en PCPlatform y hace push back en la lista de eventos
	virtual bool receiveEvent(const SDL_Event& event);
	///se anyade como listener de PCPlatform
	void init();
	///devuelve una copia de la lista de eventos producidos desde la ultima llamada a este metodo y borra la lista original
	std::list <SDL_Event> getEvents();
};

