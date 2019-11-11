#include "Fire.h"
#include <Renderer/Renderer.h>
#include <random>

int _paletaFuego[] = {
				 RGB(0x00,0x00,0x00),
				 RGB(0x07,0x07,0x07),
				 RGB(0x1F,0x07,0x07),
				 RGB(0x2F,0x0F,0x07),
				 RGB(0x47,0x0F,0x07),
				 RGB(0x57,0x17,0x07),
				 RGB(0x67,0x1F,0x07),
				 RGB(0x77,0x1F,0x07),
				 RGB(0x8F,0x27,0x07),
				 RGB(0x9F,0x2F,0x07),
				 RGB(0xAF,0x3F,0x07),
				 RGB(0xBF,0x47,0x07),
				 RGB(0xC7,0x47,0x07),
				 RGB(0xDF,0x4F,0x07),
				 RGB(0xDF,0x57,0x07),
				 RGB(0xDF,0x57,0x07),
				 RGB(0xD7,0x5F,0x07),
				 RGB(0xD7,0x5F,0x07),
				 RGB(0xD7,0x67,0x0F),
				 RGB(0xCF,0x6F,0x0F),
				 RGB(0xCF,0x77,0x0F),
				 RGB(0xCF,0x7F,0x0F),
				 RGB(0xCF,0x87,0x17),
				 RGB(0xC7,0x87,0x17),
				 RGB(0xC7,0x8F,0x17),
				 RGB(0xC7,0x97,0x1F),
				 RGB(0xBF,0x9F,0x1F),
				 RGB(0xBF,0x9F,0x1F),
				 RGB(0xBF,0xA7,0x27),
				 RGB(0xBF,0xA7,0x27),
				 RGB(0xBF,0xAF,0x2F),
				 RGB(0xB7,0xAF,0x2F),
				 RGB(0xB7,0xB7,0x2F),
				 RGB(0xB7,0xB7,0x37),
				 RGB(0xCF,0xCF,0x6F),
				 RGB(0xDF,0xDF,0x9F),
				 RGB(0xEF,0xEF,0xC7),
				 RGB(0xFF,0xFF,0xFF)
};

void Fire::simulatePixel(int x, int y)
{
	int rnd = (rand() % 3) - 1; //rnd btw [-1, 1]
	int posX = x + rnd;
	int posY = y - 1;
	posX = clamp(posX, 0, FIRE_WIDTH);
	posY = clamp(posY, 0, FIRE_HEIGHT);
	int pixelValue = _fire[posX][posY];
	int pixelTemperature = pixelValue;

	if (pixelValue != 0) { //si no se cumple, es que tiene temperatura y se le puede restar
		pixelTemperature -= rand() % 2;
	}

	_fire[x][y] = pixelTemperature;
}

void Fire::lightFirstLineFire()
{
	for (int i = 0; i < FIRE_WIDTH; i++) {
		if (_fire[i][0] < sizeof(_paletaFuego) / sizeof(_paletaFuego[0]) - 1) {
			int rnd = rand() % 2;
			_fire[i][0] += rnd;
		}
	}
}

void Fire::extinguishFirstLineFire()
{
	for (int i = 0; i < FIRE_WIDTH; i++) {
		if (_fire[i][0] > 0) {
			int rnd = rand() % 2;
			_fire[i][0] -= rnd;
		}
	}
}

int Fire::clamp(int value, int min, int max) //[min, max)
{
	int clampedValue = value;
	if (clampedValue < min) {
		clampedValue = min;
	}
	else if (clampedValue >= max) {
		clampedValue = max - 1;
	}

	return clampedValue;
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
			_fire[i][j] = 0;
		}
	}
}

void Fire::simulateFire(int mode)
{
	if (mode == LIGHT) {
		lightFirstLineFire();
	}
	else if (mode == EXTINGUISH) {
		extinguishFirstLineFire();
	}

	//simulate
	for (int i = 0; i < FIRE_WIDTH; i++) {
		for (int j = 1; j < FIRE_HEIGHT; j++) {
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

			Renderer::putPixel(width, height, _paletaFuego[_fire[i][j]]);
		}
	}
}
