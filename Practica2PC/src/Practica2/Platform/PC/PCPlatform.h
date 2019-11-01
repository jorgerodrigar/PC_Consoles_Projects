#ifndef PCPLATFORM_H_  
#define PCPLATFORM_H_

class PCPlatform {
private:
	static bool _initialized;

	PCPlatform();

public:
	static void init();
	static void release();
	static bool tick();
};

#endif 