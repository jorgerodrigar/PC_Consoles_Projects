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
	static void Init();
	static void Release();
	static bool Tick();

	static double GetDeltaTime(); // devuelve deltaTime en segundos

	static void AddListener(InputListener* listener);
	static void RemoveListener(InputListener* listener);
	static void SendEvent(const SDL_Event& event);

	// transforma, si es necesario, dependiendo del endian del PC
	static uint32_t* ToCurrentEndian(uint32_t* endian, uint32_t size);
};

#endif 