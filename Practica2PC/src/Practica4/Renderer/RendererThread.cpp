#include "RendererThread.h"
#include <Renderer/Renderer.h>

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
				//TODO: reescalado
				RenderCommandParams params = currentCommand.params;
				for (int i = params.spriteData.srcTop; i < params.spriteData.srcBottom; i++) {
					for (int j = params.spriteData.srcLeft; j < params.spriteData.srcRight; j++) {
						Renderer::putPixel(j + params.x - params.spriteData.srcLeft, i + params.y - params.spriteData.srcTop,
							params.spriteData.image[i * params.spriteData.imageWidth + j]);
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
