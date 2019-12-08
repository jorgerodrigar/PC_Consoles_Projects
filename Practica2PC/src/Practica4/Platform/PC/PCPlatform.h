#ifndef PCPLATFORM_H_  
#define PCPLATFORM_H_

#include <stdint.h>
#include <list>

class Listener; //TODO CLASS LISTENER DESARROLLADA AQUI?
class Message;

class PCPlatform {
private:
	static bool _initialized;
	static std::list<Listener*> _listeners;
	static double _currentTime;
	
	static uint32_t* _toCurrentEndian(uint32_t* endian, uint32_t size);

	PCPlatform();

public:
	static void init();
	static void release();
	static bool tick();

	static double getDeltaTime();

	static void addListener(Listener* listener);
	static void removeListener(Listener* listener);
	static void sendMessage(const Message& message);

	static uint32_t* toCurrentEndian(uint32_t* endian, uint32_t size);
};

#endif 