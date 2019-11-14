#include "Bars.h"
#include <Renderer/Renderer.h>
#include <math.h>

// _maxWidth = lo que ocupa el numero maximo de barras que caben en el ancho de la pantalla (puede ser mayor que
// el propio ancho de la pantalla al tener barras incompletas que no quepan)
Bars::Bars(int maxHeight, int barInitialPosition) : _maxHeight(maxHeight),
_barPosition(barInitialPosition), _maxWidth(ceil(Renderer::getWindowWidth() / float(BAR_WIDTH + BAR_HSEPARATION))
	* (BAR_WIDTH + BAR_HSEPARATION))
{
}

Bars::~Bars()
{
}

// pinta una barra por completo
void Bars::renderBar(int posX, int posY)
{
	int x = 0;
	for (int i = posX; i < posX + BAR_WIDTH; i++) {
		for (int j = posY; j < posY + BAR_HEIGHT; j++) {
			x = i;
			if (x > _maxWidth) { // toroide
				x -= _maxWidth + 1;
			}

			if (x >= 0 && x < Renderer::getWindowWidth()) // no se pinta si el pixel no esta dentro de la pantalla
				Renderer::putPixel(x, j, BAR_COLOR);
		}
	}
}

// pinta una barra con delta (pone pixeles negros a su izq y blancos a la der para simular su movimiento)
// delta coincide con el numero de buffers, por lo que sera el retraso con respecto a frame buffers anteriores -> coherencia de buffers
void Bars::renderBarWithDelta(int iniPosX, int posY, int delta)
{
	int finPosX = iniPosX + BAR_WIDTH;
	for (int i = 0; i < delta; i++) {
		for (int j = posY; j < posY + BAR_HEIGHT; j++) {
			if (iniPosX + i > _maxWidth) { // toroide
				iniPosX -= _maxWidth + 1;
			}
			// no se pinta si el pixel no esta dentro de la pantalla
			if (iniPosX + i >= 0 && iniPosX + i < Renderer::getWindowWidth())
				Renderer::putPixel(i + iniPosX, j, 0x000000);
			if (finPosX + i > _maxWidth) { // toroide
				finPosX -= _maxWidth + 1;
			}
			// no se pinta si el pixel no esta dentro de la pantalla
			if (finPosX + i >= 0 && finPosX + i < Renderer::getWindowWidth())
				Renderer::putPixel(i + finPosX, j, BAR_COLOR);
		}
	}
}

// pinta todas las barras a partir de posX. Esto sera llamado una vez frame buffer al inicio del main,
// cada uno con una posicion mas hacia la derecha que el anterior
void Bars::renderBars(int posX)
{
	if (posX > _maxWidth) posX = 0;

	for (int i = 0; i < Renderer::getWindowWidth(); i += BAR_HSEPARATION + BAR_WIDTH) {
		for (int j = 0; j < _maxHeight; j += BAR_VSEPARATION + BAR_HEIGHT) {
			renderBar(i + posX, j);
		}
	}

}

// pìnta todas las barras usando el delta y la coherencia entre los frame buffers anteriores
// llamado en el bucle principal
void Bars::renderBarsWithDelta()
{
	if (_barPosition > _maxWidth) _barPosition = 0;

	for (int i = 0; i < Renderer::getWindowWidth(); i += BAR_HSEPARATION + BAR_WIDTH) {
		for (int j = 0; j < _maxHeight; j += BAR_VSEPARATION + BAR_HEIGHT) {
			renderBarWithDelta(i + _barPosition, j, Renderer::getNumBuffers()); // delta = numBuffers
		}
	}

	_barPosition++;
}
