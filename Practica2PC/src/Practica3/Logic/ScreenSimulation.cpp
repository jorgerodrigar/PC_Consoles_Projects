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

	if (isValid(y, x + 1))
		sumaVecinos += previous[y * 1280 + x + 1] >> 1;
	if (isValid(y, x - 1))
		sumaVecinos += previous[y * 1280 + x - 1] >> 1;
	if (isValid(y + 1, x))
		sumaVecinos += previous[(y + 1) * 1280 + x] >> 1;
	if (isValid(y - 1, x))
		sumaVecinos += previous[(y - 1) * 1280 + x] >> 1;

	int total = (sumaVecinos - current[y * 1280 + x]);
	total -= total >> 5;

	current[y * 1280 + x] = total;
}

void ScreenSimulation::simulateRain()
{
	for (int i = 0; i < 300; i++) {
		for (int j = 0; j < 300; j++) {
			simulatePixel(j, i);
		}
	}

	RendererThread::RenderCommand command;
	command.type = RendererThread::WRITE_RAIN;
	command.params.radius = 100;
	command.params.putPixelParam.x = 100;
	command.params.putPixelParam.y = 100;
	command.params.image = image;
	command.params.current = current;
	_rendererThread->enqueueCommand(command);
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
	int rndX = rand() % 300;
	int rndY = rand() % 300;
	int rndH = rand() % 27000 + 3000;
	//current[rndY * 1280 + rndX] = rndH;
	current[100 * 1280 + 100] = rndH;
}


