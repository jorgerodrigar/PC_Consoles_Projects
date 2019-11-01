#pragma once

#define LIGHT 1
#define SIMULATE 0
#define EXTINGUISH -1
#define FIRE_WIDTH  200
#define FIRE_HEIGHT 200

class Fire
{
private:
	void simulatePixel(int x, int y);
	int fire[FIRE_WIDTH][FIRE_HEIGHT];
	void changeFirstLineFire(int mode);
	int paletaFuego[3];

public:
	Fire();
	~Fire();

	void initFire();
	void simulateFire(int mode);
	void renderFire();
};
