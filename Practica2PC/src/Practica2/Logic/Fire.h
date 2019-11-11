#pragma once

#define LIGHT 1
#define SIMULATE 0
#define EXTINGUISH -1
#define FIRE_WIDTH  800
#define FIRE_HEIGHT 100

#define RGB(r, g, b) ((r & 0xff) << 16) + ((g & 0xff) << 8) + ((b & 0xff))

class Fire
{
private:
	int _fire[FIRE_WIDTH][FIRE_HEIGHT];

	void simulatePixel(int x, int y);
	void lightFirstLineFire();
	void extinguishFirstLineFire();
	int clamp(int value, int min, int max);

public:
	Fire();
	~Fire();

	void initFire();
	void simulateFire(int mode);
	void renderFire();
};
