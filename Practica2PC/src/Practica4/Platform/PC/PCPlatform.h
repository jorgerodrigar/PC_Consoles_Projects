#ifndef PCPLATFORM_H_  
#define PCPLATFORM_H_
#include <stdint.h>
#include <Utils/Emitter.h>

class PCPlatform : public Emitter{
private:
	static bool _initialized;
	static uint32_t* _toCurrentEndian(uint32_t* endian, uint32_t size);
	
	PCPlatform();

public:
	static void init();
	static void release();
	static bool tick();

	static uint32_t* toCurrentEndian(uint32_t* endian, uint32_t size);
};

#endif 