#include "RendererThread.h"
#include <Renderer/Renderer.h>
#include <Logic/ScreenSimulation.h>

void RendererThread::renderLoop()
{
	while (!_quitRequested) {
		//pintar frame
		RenderCommand currentCommand = _concurrentQueue.pop();

		// esperamos a que termine el frame (mala idea acabar de forma abrupta)
		while (currentCommand.type != END_FRAME && !_quitRequested) {
			switch (currentCommand.type)
			{
			case CLEAR:
				Renderer::clear(currentCommand.params.color);
				break;
			case PUT_PIXEL:
			{
				RenderCommandParams params = currentCommand.params;
				Renderer::putPixel(params.putPixelParams.x, params.putPixelParams.y, params.color);
				break;
			}
			case DRAW_BACKGROUND: // llama putPixel por cada pixel de la imagen 
			{                     // (un solo comando mejor que muchos comandos de putPixel)
				RenderCommandParams params = currentCommand.params;
				for (int i = 0; i < Renderer::getWindowHeight(); i++) {
					for (int j = 0; j < Renderer::getWindowWidth(); j++) {
						Renderer::putPixel(j, i, params.simulationData.image[i * 1280 + j]);
					}
				}
				break;
			}
			case WRITE_RAIN:
			{
				RenderCommandParams params = currentCommand.params;
				renderRain(params.simulationData.image, params.simulationData.increments);
				break;
			}
			default:
				break;
			}

			currentCommand = _concurrentQueue.pop();
		}
		Renderer::present();
		_pendingframes--; //ha acabado el frame
	}
}

RendererThread::RendererThread() : _thread(nullptr), _pendingframes(0), _quitRequested(true)
{
}


RendererThread::~RendererThread()
{
}

void RendererThread::start()
{
	_quitRequested = false;

	if (_thread != nullptr) return;

	_thread = new std::thread(&RendererThread::renderLoop, this);
}

void RendererThread::stop()
{
	_quitRequested = true;

	if (_thread == nullptr) return;

	RenderCommand command;
	command.type = END_FRAME;
	_concurrentQueue.push(command);

	_thread->join(); delete _thread; _thread = nullptr;
}

void RendererThread::enqueueCommand(RenderCommand c)
{
	if (c.type == END_FRAME)
		_pendingframes++;
	//enqueue
	_concurrentQueue.push(c);
}

// recorre el array de incrementos para pintar solo los pixeles que hayan cambiado
// con respecto al buffer anterior -> coherencia de buffers
void RendererThread::renderRain(int* image, int* increments)
{
	for (int i = 0; i < MATRIX_HEIGHT; i++) {
		for (int j = 0; j < MATRIX_WIDTH; j++) {
			int incr = increments[i * MATRIX_WIDTH + j];
			if (incr & 1) { // ultimo bit a 1 -> ha cambiado respecto al anterior y hay que pintarlo
				incr >>= 1; // recuperamos el valor original
				int rgba[4];
				Renderer::hexToRGBA(image[i * MATRIX_WIDTH + j], rgba);

				for (int c = 0; c < 4; c++) {                  // para cada componente de color
					rgba[c] = clamp(rgba[c] - (incr), 0, 256); // clamp(componente - (iz - der), 0, 256)
				}

				Renderer::putPixel(j, i, RGBA(rgba[0], rgba[1], rgba[2], rgba[3]));
			}
		}
	}
}

int RendererThread::clamp(int value, int min, int max) //[min, max)
{
	int clampedValue = value;
	if (clampedValue < min) {
		clampedValue = min;
	}
	else if (clampedValue >= max) {
		clampedValue = max - 1;
	}

	return clampedValue;
}
