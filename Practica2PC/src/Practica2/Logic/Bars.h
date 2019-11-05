#pragma once

#define BAR_HEIGHT 10
#define BAR_WIDTH 100
#define BAR_HSEPARATION 20
#define BAR_VSEPARATION 20
#define BAR_COLOR 0xFFFFFF

class Bars
{
private:
	int _x_barra = 0;
	const int _maxHeight;

	void renderBar(int posX, int posY);
	void renderBarWithDelta(int posX, int posY, int delta);

public:
	Bars(int maxHeight);
	~Bars();
	void renderBars(int posX);
	void renderBarsWithDelta(int posX);
};

