#pragma once

#include <Utils/RendererThread.h>

#define TEST_DIMENSIONS 300
class ScreenSimulation
{
private:
	int *current, *previous;
	RendererThread* _rendererThread;

	void simulatePixel(int x, int y);
	void renderPixel(int x, int y);
	int clamp(int value, int min, int max);
	bool isValid(int i, int j);
	void swap(int*& a, int*& b);
public:
	ScreenSimulation();
	~ScreenSimulation();
	void init(const char* filePath, RendererThread* rendererThread); //lee el archivo y guarda los pixeles en image
	void drawBackground();
	void render();
	void simulateRain();
	void startRandomWave();
};

