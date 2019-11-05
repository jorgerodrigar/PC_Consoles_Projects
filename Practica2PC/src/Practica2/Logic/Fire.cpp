#include "Fire.h"
#include <Renderer/Renderer.h>
#include <random>

//TODO: PREGUNTAR MACRO A PEPA O MARCO
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
		if (_fire[i][0] < 38) {
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

	_paletaFuego[0] = 0x000000;
	_paletaFuego[1] = 0x070707;
	_paletaFuego[2] = 0x1F0707;
	_paletaFuego[3] = 0x2F0F07;
	_paletaFuego[4] = 0x470F07;
	_paletaFuego[5] = 0x571707;
	_paletaFuego[6] = 0x671F07;
	_paletaFuego[7] = 0x771F07;
	_paletaFuego[8] = 0x8F2707;
	_paletaFuego[9] = 0x9F2F07;
	_paletaFuego[10] = 0xAF3F07;
	_paletaFuego[11] = 0xBF4707;
	_paletaFuego[12] = 0xC74707;
	_paletaFuego[13] = 0xDF4F07;
	_paletaFuego[14] = 0xDF5707;
	_paletaFuego[15] = 0xDF5707;
	_paletaFuego[16] = 0xD75F07;
	_paletaFuego[17] = 0xD75F07;
	_paletaFuego[18] = 0xD7670F;
	_paletaFuego[19] = 0xCF6F0F;
	_paletaFuego[20] = 0xCF770F;
	_paletaFuego[21] = 0xCF7F0F;
	_paletaFuego[22] = 0xCF8717;
	_paletaFuego[23] = 0xC78717;
	_paletaFuego[24] = 0xC78F17;
	_paletaFuego[25] = 0xC7971F;
	_paletaFuego[26] = 0xBF9F1F;
	_paletaFuego[27] = 0xBF9F1F;
	_paletaFuego[28] = 0xBFA727;
	_paletaFuego[29] = 0xBFA727;
	_paletaFuego[30] = 0xBFAF2F;
	_paletaFuego[31] = 0xB7AF2F;
	_paletaFuego[32] = 0xB7B72F;
	_paletaFuego[33] = 0xB7B737;
	_paletaFuego[34] = 0xCFCF6F;
	_paletaFuego[35] = 0xDFDF9F;
	_paletaFuego[36] = 0xEFEFC7;
	_paletaFuego[37] = 0xFFFFFF;
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
