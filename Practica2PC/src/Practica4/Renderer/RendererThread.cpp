#include "RendererThread.h"
#include <Renderer/Renderer.h>
#include <iostream>

#define SCALE_FACTOR 3

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
				Renderer::putPixel(params.x, params.y, params.color);
				break;
			}
			case DRAW_SPRITE:
			{
				RenderCommandParams params = currentCommand.params;
				for (int i = params.spriteData.srcTop; i < params.spriteData.srcBottom; i++) {
					for (int j = params.spriteData.srcLeft; j < params.spriteData.srcRight; j++) {
						drawRescaled(i, j, params, SCALE_FACTOR); // pintaremos todas las imagenes reescaladas
					}
				}
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

// por cada pixel en j, i, lo pintamos reescalado a scale * scale
void RendererThread::drawRescaled(int i, int j, const RenderCommandParams& params, const int scale)
{
	int color = params.spriteData.image[i * params.spriteData.imageWidth + j]; // color del pixel a reescalar
	int  rgba[4];
	Renderer::hexToRGBA(color, rgba);

	int true_color = RGBA(rgba[0], rgba[1], rgba[2], rgba[3]);

	// pintamos el pixel repetido en un cuadrado de scale * scale que empieza en j, i
	int y = i * scale;
	for (y; y < (i * scale) + scale; y++) {
		int x = j * scale;
		for (x; x < (j * scale) + scale; x++) {
			Renderer::putPixel(x + params.x - (params.spriteData.srcLeft * scale), 
				y + params.y - (params.spriteData.srcTop*scale), true_color);
		}
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
