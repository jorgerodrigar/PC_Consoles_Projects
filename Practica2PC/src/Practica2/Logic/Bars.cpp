#include "Bars.h"
#include <Renderer/Renderer.h>

Bars::Bars(int maxHeight): _maxHeight(maxHeight)
{
}


Bars::~Bars()
{
}

void Bars::renderBar(int posX, int posY)
{
	int x = 0;
	for (int i = posX; i < posX + BAR_WIDTH; i++) {
		for (int j = posY; j < posY + BAR_HEIGHT; j++) {
			x = i;
			if(i > Renderer::getWindowWidth()){
				x = i - Renderer::getWindowWidth();
			}
			Renderer::putPixel(x, j, BAR_COLOR);
		}
	}
}

void Bars::renderBarWithDelta(int posX, int posY, int delta)
{
	for (int i = 0; i < delta; i++) {
		for (int j = posY; j < posY + BAR_HEIGHT; j++) {
			if (posX > Renderer::getWindowWidth()) {
				posX = posX - Renderer::getWindowWidth() - BAR_WIDTH;
			}
			Renderer::putPixel(posX - (delta - i), j, 0x000000);
			Renderer::putPixel(i + posX + BAR_WIDTH, j, BAR_COLOR);
		}
	}
}

void Bars::renderBars(int posX)
{
	for (int i = posX; i < Renderer::getWindowWidth() + posX; i += BAR_HSEPARATION + BAR_WIDTH) {
		for (int j = 0; j < _maxHeight; j += BAR_VSEPARATION + BAR_HEIGHT) {
			renderBar(i, j);
		}
	}
}

void Bars::renderBarsWithDelta(int posX)
{
	for (int i = 0; i < Renderer::getWindowWidth(); i += BAR_HSEPARATION + BAR_WIDTH) {
		for (int j = 0; j < _maxHeight; j += BAR_VSEPARATION + BAR_HEIGHT) {
			renderBarWithDelta(i + posX, j, 1); //1 == renderer::getnumbuffers()
		}
	}
}
