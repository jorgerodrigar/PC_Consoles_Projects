#include "ScreenSimulation.h"
#include <Utils/BinReader.h>
#include <Renderer/Renderer.h>
#include <Utils/RendererThread.h>
#include <iostream>

int image[MATRIX_HEIGHT * MATRIX_WIDTH];   // fondo

// matrices de simulacion que se iran intercambiando mediante los punteros current y previous
int matrix0[MATRIX_HEIGHT * MATRIX_WIDTH];
int matrix1[MATRIX_HEIGHT * MATRIX_WIDTH];

// calcula la altura del pixel en la posicion x, y
void ScreenSimulation::simulatePixel(int x, int y)
{
	int sumaVecinos = 0;

	// suma de vecinos de la matriz anterior (izq, der, arriba, abajo) / 2
	if (isValid(y, x + 1))
		sumaVecinos += previous[y * MATRIX_WIDTH + x + 1] >> 1;
	if (isValid(y, x - 1))
		sumaVecinos += previous[y * MATRIX_WIDTH + x - 1] >> 1;
	if (isValid(y + 1, x))
		sumaVecinos += previous[(y + 1) * MATRIX_WIDTH + x] >> 1;
	if (isValid(y - 1, x))
		sumaVecinos += previous[(y - 1) * MATRIX_WIDTH + x] >> 1;

	// suma de vecinos menos posicion del pixel en la matriz actual, con un desplazamiento para disminuir su altura
	int total = (sumaVecinos - current[y * MATRIX_WIDTH + x]);
	total -= total >> 3;

	current[y * MATRIX_WIDTH + x] = total;
}

// simula todas las gotas en pantalla y calcula sus incrementos para disminuir la carga de putPixels de RenderThread
void ScreenSimulation::simulateRain()
{
	for (int i = 0; i < MATRIX_HEIGHT; i++) {
		for (int j = 0; j < MATRIX_WIDTH; j++) {
			simulatePixel(j, i);
		}
	}
	calculateIncrement();

	// comando de pintar la lluvia a la hebra
	RendererThread::RenderCommand command;
	command.type = RendererThread::WRITE_RAIN;
	command.params.simulationData.increments = _increments[delta];
	command.params.simulationData.image = image;
	_rendererThread->enqueueCommand(command);

	// aumenta el delta -> array de incrementos del buffer siguiente
	delta++;
	if (delta >= Renderer::getNumBuffers() + 1) {
		delta = 0;
	}
}

bool ScreenSimulation::isValid(int i, int j)
{
	return i > 0 && i < MATRIX_HEIGHT && j > 0 && MATRIX_WIDTH;
}

// cambio de punteros: current <-> previous
void ScreenSimulation::swap(int *& a, int *& b)
{
	int * c = a;
	a = b;
	b = c;
}

// calcula los incrementos (izq-der) de cada pixel, y compara con los incrementos del
// buffer anterior, marcando con un bit si ha habido cambio con respecto al anterior
void ScreenSimulation::calculateIncrement()
{
	for (int i = 0; i < MATRIX_HEIGHT; i++) {
		float iz = 0, der = 0;
		for (int j = 0; j < MATRIX_WIDTH; j++) {
			if (isValid(i, j - 1)) {
				iz = current[i * MATRIX_WIDTH + (j - 1)];
			}
			if (isValid(i, j + 1)) {
				der = current[i * MATRIX_WIDTH + (j + 1)];
			}
			int diff = iz - der;

			//cambios
			int prevDelta = delta + 1;
			if (prevDelta >= Renderer::getNumBuffers() + 1) prevDelta = 0;
			if (_increments[prevDelta][i * MATRIX_WIDTH + j] >> 1 != diff) {
				diff << 1;
				diff |= 1; // ultimo bit a 1 -> ha cambiado
			}
			else {
				diff << 1; // ultimo bit a 0 -> no ha cambiado
			}
			_increments[delta][i * MATRIX_WIDTH + j] = diff;
		}
	}
}

ScreenSimulation::ScreenSimulation()
{
}

ScreenSimulation::~ScreenSimulation()
{
	for (int i = 0; i < Renderer::getNumBuffers() + 1; i++) {
		delete[] _increments[i];
		_increments[i] = nullptr;
	}
}

// lee el archivo y guarda en image los colores del fondo
// inicializa las matrices a 0
void ScreenSimulation::init(const char* filePath, RendererThread* rendererThread)
{
	_rendererThread = rendererThread;

	FILE* ptr = BinReader::openFile(filePath);
	for (int i = 0; i < MATRIX_HEIGHT; i++) {
		for (int j = 0; j < MATRIX_WIDTH; j++) {
			int r = BinReader::readByte(ptr);
			int g = BinReader::readByte(ptr);
			int b = BinReader::readByte(ptr);
			int a = BinReader::readByte(ptr);
			if (r == EOF) {
				break;
			}
			image[i * MATRIX_WIDTH + j] = RGBA(r, g, b, a);

			matrix0[i * MATRIX_WIDTH + j] = 0;
			matrix1[i * MATRIX_WIDTH + j] = 0;
		}
	}

	BinReader::closeFile(ptr);

	current = matrix0;
	previous = matrix1;

	//init increments
	for (int i = 0; i < Renderer::getNumBuffers() + 1; i++) {
		_increments.push_back(new int[MATRIX_HEIGHT * MATRIX_WIDTH]);
		for (int incr = 0; incr < MATRIX_HEIGHT * MATRIX_WIDTH; incr++) {
			_increments[i][incr] = 0;
		}
	}
}

// envia a la hebra de pintado el comando para dibujar el fondo (mejor que mandar
// un comando de putPixel por cada pixel de la imagen)
void ScreenSimulation::drawBackground()
{
	RendererThread::RenderCommand command;
	command.type = RendererThread::DRAW_BACKGROUND;
	command.params.simulationData.image = image;

	_rendererThread->enqueueCommand(command);
}

void ScreenSimulation::swap()
{
	swap(current, previous);
}

void ScreenSimulation::startRandomWave()
{
	int rndX = rand() % MATRIX_WIDTH;
	int rndY = rand() % MATRIX_HEIGHT;
	int rndH = rand() % 27000 + 3000; // 3000-30000
	current[rndY * MATRIX_WIDTH + rndX] = rndH;
}


