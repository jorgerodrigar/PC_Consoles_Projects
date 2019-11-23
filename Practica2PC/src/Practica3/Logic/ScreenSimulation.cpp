#include "ScreenSimulation.h"
#include <Utils/BinReader.h>
#include <Renderer/Renderer.h>
#include <iostream>

int image[720 * 1280];
int matrix0[720 * 1280];
int matrix1[720 * 1280];

void ScreenSimulation::simulatePixel(int x, int y)
{
	int sumaVecinos = 0;
	/*for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			if (isValid(y + i, x + j)) {
				sumaVecinos += previous[(y + i) * 1280 + (x + j)] >> 1;
			}
		}
	}*/
	if (isValid(y, x + 1))
		sumaVecinos += previous[y * 1280 + x + 1] >> 1;
	if (isValid(y, x - 1))
		sumaVecinos += previous[y * 1280 + x - 1] >> 1;
	if (isValid(y + 1, x))
		sumaVecinos += previous[(y + 1) * 1280 + x] >> 1;
	if (isValid(y - 1, x))
		sumaVecinos += previous[(y - 1) * 1280 + x] >> 1;

	int total = (sumaVecinos + current[y * 1280 + x]);
	total -= total >> 5;

	current[y * 1280 + x] = total;
	/*
	current[y * 1280 + x] = sumaVecinos + current[y * 1280 + x];
	current[y * 1280 + x] = current[y * 1280 + x] - current[y * 1280 + x] >> 5;
	*/
}

void ScreenSimulation::simulateRain()
{
	for (int i = 0; i < TEST_DIMENSIONS; i++) {
		for (int j = 0; j < TEST_DIMENSIONS; j++) {
			simulatePixel(j, i);
		}
	}
}

int ScreenSimulation::clamp(int value, int min, int max) //[min, max)
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

int ScreenSimulation::renderPixel(int x, int y)
{
	int rgba[4];
	Renderer::hexToRGBA(image[y * 1280 + x], rgba);

	for (int i = 0; i < 4; i++) {
		float iz = 0, der = 0;
		if (isValid(y, x - 1)) {
			iz = current[y * 1280 + (x - 1)];
		}
		if (isValid(y, x + 1)) {
			der = current[y * 1280 + (x + 1)];
		}

		rgba[i] = clamp(rgba[i] - (iz - der), 0, 256);
	}
	
	return RGBA(rgba[0], rgba[1], rgba[2], rgba[3]);
}

bool ScreenSimulation::isValid(int i, int j)
{
	return i > 0 && i < Renderer::getWindowHeight() && j > 0 && Renderer::getWindowWidth();
}

void ScreenSimulation::swap(int *& a, int *& b)
{
	int * c = a;
	a = b;
	b = c;
}

ScreenSimulation::ScreenSimulation()
{
}

ScreenSimulation::~ScreenSimulation()
{
}

void ScreenSimulation::init(const char* filePath, RendererThread* rendererThread)
{
	_rendererThread = rendererThread;

	FILE* ptr = BinReader::openFile(filePath);
	for (int i = 0; i < Renderer::getWindowHeight(); i++) {
		for (int j = 0; j < Renderer::getWindowWidth(); j++) {
			int r = BinReader::readByte(ptr);
			int g = BinReader::readByte(ptr);
			int b = BinReader::readByte(ptr);
			int a = BinReader::readByte(ptr);
			if (r == EOF) {
				break;
			}
			image[i * 1280 + j] = RGBA(r, g, b, a);

			matrix0[i * 1280 + j] = 0;
			matrix1[i * 1280 + j] = 0;
		}
	}

	BinReader::closeFile(ptr);

	current = matrix0;
	previous = matrix1;
}

void ScreenSimulation::drawBackground()
{
	for (int i = 0; i < Renderer::getWindowHeight(); i++) {
		for (int j = 0; j < Renderer::getWindowWidth(); j++) {
			RendererThread::RenderCommand command;
			command.type = RendererThread::PUT_PIXEL;
			command.params.color = image[i * 1280 + j];
			command.params.putPixelParam.x = j;
			command.params.putPixelParam.y = i;

			_rendererThread->enqueueCommand(command);
		}
	}
}

void ScreenSimulation::swap()
{
	swap(current, previous);
}

void ScreenSimulation::startRandomWave()
{
	current[100 * 1280 + 100] = 5000;
}


