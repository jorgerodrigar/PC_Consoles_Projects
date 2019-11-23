#pragma once

#include <Utils/RendererThread.h>

class ScreenSimulation
{
private:
	int *current, *previous;
	RendererThread* _rendererThread;

	void simulatePixel(int x, int y);
	bool isValid(int i, int j);
	void swap(int*& a, int*& b);

public:
	ScreenSimulation();
	~ScreenSimulation();
	void init(const char* filePath, RendererThread* rendererThread); //lee el archivo y guarda los pixeles en image
	void drawBackground();
	void swap();
	void simulateRain();
	void startRandomWave();
};

