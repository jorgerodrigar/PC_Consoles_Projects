#pragma once

#include <vector>
#define MATRIX_WIDTH 1280
#define MATRIX_HEIGHT 720

// clase que gestiona la simulacion de las gotas
// tiene tambien la imagen del fondo

class RendererThread;
class ScreenSimulation
{
private:
	int *current, *previous;       // arrays de simulacion (alturas)
	RendererThread* _rendererThread;
	std::vector<int*> _increments; // vector de arrays con los incrementos (diferencias entre izq y der)
	int delta = 0;                 // indicador del array de incremento actualmente visitado

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

