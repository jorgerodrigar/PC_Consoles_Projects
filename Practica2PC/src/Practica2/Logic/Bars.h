#pragma once

#define BAR_HEIGHT 10
#define BAR_WIDTH 100
#define BAR_HSEPARATION 20
#define BAR_VSEPARATION 20
#define BAR_COLOR 0xFFFFFF

// esta clase pinta todas las barras que habra en pantalla
class Bars
{
private:
	const int _maxWidth;
	const int _maxHeight;

	int _barPosition = 0;

	void renderBar(int posX, int posY);
	void renderBarWithDelta(int posX, int posY, int delta);

public:
	Bars(int maxHeight, int barInitialPosition);
	~Bars();
	void renderBars(int posX);
	void renderBarsWithDelta();
};

