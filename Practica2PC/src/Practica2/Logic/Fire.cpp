#include "Fire.h"
#include <Renderer/Renderer.h>
#include <random>

//TODO: PENSAR X E Y PUESTAS AL REVES
//TODO: PREGUNTAR MACRO A PEPA O MARCO
//TODO: CAMBIAR MODOS Y CONTAR FRAMES
void Fire::simulatePixel(int x, int y)
{
	int rnd = (rand() % 3) - 1; //rnd btw [-1, 1]
	int pixelValue = fire[x + rnd][y - 1];
	int pixelTemperature = pixelValue;

	if (pixelValue != 0) {
		pixelTemperature -= rand() % 2;
	}

	fire[x][y] = pixelTemperature;
}

void Fire::changeFirstLineFire(int mode)
{
	for (int i = 0; i < FIRE_WIDTH; i++) {
		int rnd = rand() % 2;
		fire[i][0] += rnd * mode;
	}
}

Fire::Fire()
{
}


Fire::~Fire()
{
}

void Fire::initFire()
{
	for (int i = 0; i < FIRE_WIDTH; i++) {
		for (int j = 0; j < FIRE_HEIGHT; j++) {
			fire[i][j] = 0;
		}
	}

	paletaFuego[0] = 0x00000;
	paletaFuego[1] = 0xBFA72F;
	paletaFuego[2] = 0xFFFFFF;
}

void Fire::simulateFire(int mode)
{
	if (mode != SIMULATE) {
		//extinguish or light the fire
		changeFirstLineFire(mode);
	}

	//simulate
	for (int i = 1; i < FIRE_WIDTH - 1; i++) {
		for (int j = 1; j < FIRE_HEIGHT - 1; j++) {
			simulatePixel(i, j);
		}
	}
}

void Fire::renderFire()
{
	for (int i = 0; i < FIRE_WIDTH; i++) {
		for (int j = 0; j < FIRE_HEIGHT; j++) {
			int width = (Renderer::getWindowWidth() / 2) - (FIRE_WIDTH / 2) + i;
			int height = Renderer::getWindowHeight() - j;

			Renderer::putPixel(width, height, paletaFuego[fire[i][j]]);
		}
	}
}
