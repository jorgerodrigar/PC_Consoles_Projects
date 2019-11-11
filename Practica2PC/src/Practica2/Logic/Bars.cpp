#include "Bars.h"
#include <Renderer/Renderer.h>
#include <math.h>

Bars::Bars(int maxHeight, int barInitialPosition) : _maxHeight(maxHeight),
_barPosition(barInitialPosition), _maxWidth(ceil(Renderer::getWindowWidth() / float(BAR_WIDTH + BAR_HSEPARATION))
	* (BAR_WIDTH + BAR_HSEPARATION))
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
			if (i > _maxWidth) {
				x = i - _maxWidth;
			}
			Renderer::putPixel(x, j, BAR_COLOR);
		}
	}
}

void Bars::renderBarWithDelta(int iniPosX, int posY, int delta)
{
	int finPosX = iniPosX + BAR_WIDTH;
	for (int i = 0; i < delta; i++) {
		for (int j = posY; j < posY + BAR_HEIGHT; j++) {
			if (iniPosX > _maxWidth) {    // if (iniPosX + i > _maxWidth)
				iniPosX -= _maxWidth + 1; // iniPosX -= _maxWidth + i + 1;
			}
			if (iniPosX + i >= 0 && iniPosX + i <= _maxWidth)
				Renderer::putPixel(i + iniPosX, j, 0x000000);
			if (finPosX > _maxWidth) {
				finPosX -= _maxWidth + 1;
			}
			if (finPosX + i >= 0 && finPosX + i <= _maxWidth)
				Renderer::putPixel(i + finPosX, j, BAR_COLOR);
		}
	}
}

void Bars::renderBars(int posX)
{
	if (posX > _maxWidth) posX = 0;

	for (int i = 0; i < Renderer::getWindowWidth(); i += BAR_HSEPARATION + BAR_WIDTH) {
		for (int j = 0; j < _maxHeight; j += BAR_VSEPARATION + BAR_HEIGHT) {
			renderBar(i + posX, j);
		}
	}

}

void Bars::renderBarsWithDelta()
{
	if (_barPosition > _maxWidth) _barPosition = 0;

	for (int i = 0; i < Renderer::getWindowWidth(); i += BAR_HSEPARATION + BAR_WIDTH) {
		for (int j = 0; j < _maxHeight; j += BAR_VSEPARATION + BAR_HEIGHT) {
			renderBarWithDelta(i + _barPosition, j, Renderer::getNumBuffers());
		}
	}

	_barPosition++;
}
