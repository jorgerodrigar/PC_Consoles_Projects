#pragma once

#include <vector>
#define MATRIX_WIDTH 1280
#define MATRIX_HEIGHT 720

class RendererThread;
class ScreenSimulation
{
private:
	int *current, *previous;
	RendererThread* _rendererThread;
	std::vector<int*> _increments;
	int delta = 0;

	void simulatePixel(int x, int y);
	bool isValid(int i, int j);
	void swap(int*& a, int*& b);
	void calculateIncrement();

public:
	ScreenSimulation();
	~ScreenSimulation();
	void init(const char* filePath, RendererThread* rendererThread); //lee el archivo y guarda los pixeles en image
	void drawBackground();
	void swap();
	void simulateRain();
	void startRandomWave();
};

