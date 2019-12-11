#ifndef PCPLATFORM_H_  
#define PCPLATFORM_H_

#include <stdint.h>
#include <list>
#include <SDL_events.h>

class InputListener;

class PCPlatform {
private:
	static bool _initialized;
	static std::list<InputListener*> _listeners;
	static double _currentTime;
	
	static uint32_t* _toCurrentEndian(uint32_t* endian, uint32_t size);

	PCPlatform();

public:
	static void init();
	static void release();
	static bool tick();

	static double getDeltaTime();

	static void addListener(InputListener* listener);
	static void removeListener(InputListener* listener);
	static void sendEvent(const SDL_Event& event);

	static uint32_t* toCurrentEndian(uint32_t* endian, uint32_t size);
};

#endif 