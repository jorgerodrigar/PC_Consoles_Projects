#include "RendererThread.h"
#include <Renderer/Renderer.h>
#include <Logic/ScreenSimulation.h>

void RendererThread::renderLoop()
{
	while (!_quitRequested) {
		//pintar frame
		RenderCommand currentCommand = _concurrentQueue.pop();
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
			case DRAW_BACKGROUND:
			{
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
				renderRain(params.simulationData.current, params.simulationData.image, params.simulationData.increments);
				/*RenderCommandParams params = currentCommand.params;
				float radius = 100;
				for (int i = -radius; i < radius; i++) {
					for (int j = -radius; j < radius; j++) {
						int x = i + params.putPixelParams.x;
						int y = j + params.putPixelParams.y;

						Renderer::putPixel(x, y, renderPixel(x, y, params.simulationData.current, params.simulationData.image));
					}
				}
*/
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

RendererThread::RendererThread()
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
	_concurrentQueue.push(command); //	TODO: PROVISIONAL, AQUI O EN MAIN??

	_thread->join(); delete _thread; _thread = nullptr;
}

void RendererThread::enqueueCommand(RenderCommand c)
{
	if (c.type == END_FRAME) 
		_pendingframes++;
	//enqueue
	_concurrentQueue.push(c);
}


void RendererThread::renderRain(int* current, int* image, int* increments)
{
	for (int i = 0; i < MATRIX_HEIGHT; i++) {
		for (int j = 0; j < MATRIX_WIDTH; j++) {
			int incr = increments[i * MATRIX_WIDTH + j];
			if (incr & 1) {
				incr >>= 1;
				int rgba[4];
				Renderer::hexToRGBA(image[i * MATRIX_WIDTH + j], rgba);

				for (int c = 0; c < 4; c++) {
					rgba[c] = clamp(rgba[c] - (incr), 0, 256);
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
