#ifndef PCPLATFORM_H_  
#define PCPLATFORM_H_

#include <stdint.h>
#include <list>
#include <SDL_events.h>

const std::string HEADER_PATH = "";

class InputListener;

class PCPlatform {
private:
	static bool _initialized;
	static std::list<InputListener*> _listeners;
	static double _currentTime;
	
	static uint32_t* _toCurrentEndian(uint32_t* endian, uint32_t size);

	PCPlatform();

public:
	///inicializa todo el sistema de SDL
	static void Init();
	///libera todo el sistema de SDL
	static void Release();
	///detecta eventos y los propaga a sus listeners. Maneja los eventos de cierre de la aplicacion (boton de salir, clickar en la pantalla)
	static bool Tick();

	/// devuelve deltaTime en segundos
	static double GetDeltaTime();

	static void AddListener(InputListener* listener);
	static void RemoveListener(InputListener* listener);
	/// manda un evento a todos sus listeners. Si alguno de ellos lo procesa, deja de propagarlo.
	static void SendEvent(const SDL_Event& event);

	/// transforma, si es necesario, dependiendo del endian del PC
	static uint32_t* ToCurrentEndian(uint32_t* endian, uint32_t size);
};

#endif 