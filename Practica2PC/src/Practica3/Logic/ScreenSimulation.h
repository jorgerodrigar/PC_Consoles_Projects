#pragma once
class ScreenSimulation
{
private:
	int** image, **matrix0, **matrix1;
	int currentMatrix = 0;

	void simulatePixel(int x, int y);
	void renderPixel(int x, int y);
	int clamp(int value, int min, int max);
	bool isValid(int i, int j, int** matrix);
public:
	ScreenSimulation();
	~ScreenSimulation();
	void init(const char* filePath); //lee el archivo y guarda los pixeles en image
	void drawBackground();
	void render();
};

