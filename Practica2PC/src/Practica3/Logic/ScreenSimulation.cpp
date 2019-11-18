#include "ScreenSimulation.h"
#include <Utils/BinReader.h>
#include <Renderer/Renderer.h>
#include <iostream>

void ScreenSimulation::simulatePixel(int x, int y)
{
	if (currentMatrix == 0) {
		//valor de propio pixel = matrix0; valor vecinos = matrix1
		float sumaVecinos = 0;
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				if (isValid(y + i, x + j)) {
					sumaVecinos += matrix1[y + i][x + j] * 0.5f;
				}
			}
		}
		float total = (sumaVecinos + matrix0[y][x]) * 31.0f / 32.0f;
		total = clamp(total, 0, 256);

		matrix0[y][x] = total;
	}
	else { //TODO: solucionar repeticion de codigo
		//valor de propio pixel = matrix1; valor vecinos = matrix0
		float sumaVecinos = 0;
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				if (isValid(y + i, x + j)) {
					sumaVecinos += matrix0[y + i][x + j] * 0.5f;
				}
			}
		}
		float total = (sumaVecinos + matrix1[y][x]) * 31.0f / 32.0f;
		total = clamp(total, 0, 256);

		matrix1[y][x] = total;
	}
}

void ScreenSimulation::simulateRain()
{
	for (int i = 0; i < 300; i++) {
		for (int j = 0; j < 300; j++) {
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

void ScreenSimulation::renderPixel(int x, int y)
{
	int* rgba = Renderer::hexToRGBA(image[y][x]);
	int aux[] = { rgba[0], rgba[1], rgba[2], rgba[3] };
	int aux2[] = { rgba[0], rgba[1], rgba[2], rgba[3] };

	for (int i = 0; i < 4; i++) {
		float iz = 0, der = 0;
		if (isValid(y, x - 1)) {
			iz = currentMatrix == 0 ? matrix0[y][x - 1] : matrix1[y][x - 1];
		}
		if (isValid(y, x + 1)) {
			der = currentMatrix == 0 ? matrix0[y][x + 1] : matrix1[y][x + 1];
		}

		aux[i] = clamp(aux[i] - (iz - der), 0, 256);
	}

	//if(matrix0[y][x] != matrix1[y][x])
		Renderer::putPixel(x, y, RGBA(aux[0], aux[1], aux[2], aux[3]));
}

bool ScreenSimulation::isValid(int i, int j)
{
	return i > 0 && i < Renderer::getWindowHeight() && j > 0 && Renderer::getWindowWidth();
}

ScreenSimulation::ScreenSimulation()
{
	image = new int*[Renderer::getWindowHeight()];
	matrix0 = new float*[Renderer::getWindowHeight()];
	matrix1 = new float*[Renderer::getWindowHeight()];

	for (int i = 0; i < Renderer::getWindowHeight(); i++) {
		image[i] = new int[Renderer::getWindowWidth()];
		matrix0[i] = new float[Renderer::getWindowWidth()];
		matrix1[i] = new float[Renderer::getWindowWidth()];
	}
}


ScreenSimulation::~ScreenSimulation()
{
	for (int i = 0; i < Renderer::getWindowHeight(); i++) {
		delete[] image[i];
		delete[] matrix0[i];
		delete[] matrix1[i];
	}

	delete[] image;
	delete[] matrix0;
	delete[] matrix1;
}

void ScreenSimulation::init(const char* filePath)
{
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
			image[i][j] = RGBA(r, g, b, a);

			matrix0[i][j] = 0;
			matrix1[i][j] = 0;
		}
	}

	BinReader::closeFile(ptr);
}

void ScreenSimulation::drawBackground()
{
	for (int i = 0; i < Renderer::getWindowHeight(); i++) {
		for (int j = 0; j < Renderer::getWindowWidth(); j++) {
			Renderer::putPixel(j, i, image[i][j]);
		}
	}
}

void ScreenSimulation::render()
{
	for (int i = 0; i < 300; i++) {
		for (int j = 0; j < 300; j++) {
			renderPixel(j, i);
		}
	}

	currentMatrix = !currentMatrix;
}

void ScreenSimulation::startRandomWave()
{
	currentMatrix == 0 ? matrix0[100][100] = 255 : matrix1[100][100] = 255;
}


