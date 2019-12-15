#include "RendererThread.h"
#include <Renderer/Renderer.h>
#include <iostream>

void RendererThread::renderLoop()
{
	while (!_quitRequested) {
		//pintar frame
		RenderCommand currentCommand = _concurrentQueue.pop();
		while (currentCommand.type != END_FRAME && !_quitRequested) {
			switch (currentCommand.type)
			{
			case CLEAR:
				for(int i = 0; i < Renderer::GetNumBuffers(); i++) 
					Renderer::Clear(currentCommand.params.color);
				break;
			case PUT_PIXEL:
			{
				RenderCommandParams params = currentCommand.params;
				Renderer::PutPixel(params.x, params.y, params.color);
				break;
			}
			case DRAW_spriteSheet:
			{
				RenderCommandParams params = currentCommand.params;
				for (int i = params.SpriteSheetData.srcTop; i < params.SpriteSheetData.srcBottom; i++) {
					for (int j = params.SpriteSheetData.srcLeft; j < params.SpriteSheetData.srcRight; j++) {
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
		Renderer::Present();
		_pendingframes--; //ha acabado el frame
	}
}

// por cada pixel en j, i, lo pintamos reescalado a scale * scale
void RendererThread::drawRescaled(int i, int j, const RenderCommandParams& params, const int scale)
{
	int color = params.SpriteSheetData.image[i * params.SpriteSheetData.imageWidth + j]; // color del pixel a reescalar
	int  rgba[4];
	Renderer::HexToRGBA(color, rgba);

	int true_color = RGBA(rgba[0], rgba[1], rgba[2], rgba[3]);

	// pintamos el pixel repetido en un cuadrado de scale * scale que empieza en j, i
	int y = i * scale;
	for (y; y < (i * scale) + scale; y++) {
		int x = j * scale;
		for (x; x < (j * scale) + scale; x++) {
			//if ((x + params.x - (params.SpriteSheetData.srcLeft * scale) >= 0 && (x + params.x - (params.SpriteSheetData.srcLeft * scale) < Renderer::getWindowWidth()))) { //todo: esta bien aqui??
				Renderer::PutPixel(x + (params.x * scale) - (params.SpriteSheetData.srcLeft * scale),
					y + (params.y * scale) - (params.SpriteSheetData.srcTop*scale), true_color);
			//}
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
