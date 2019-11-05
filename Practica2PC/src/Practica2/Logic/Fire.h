#pragma once

#define LIGHT 1
#define SIMULATE 0
#define EXTINGUISH -1
#define FIRE_WIDTH  400
#define FIRE_HEIGHT 100

class Fire
{
private:
	int _fire[FIRE_WIDTH][FIRE_HEIGHT];
	int _paletaFuego[38];

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
