#pragma once
#include <thread>
#include <atomic>
#include <Utils/ConcurrentQueue.h>
#include <stdint.h>

#define SCALE_FACTOR 2

class RendererThread
{
public:
	enum RenderCommandType { CLEAR, END_FRAME, PUT_PIXEL, DRAW_spriteSheet };

	struct SpriteSheetData {
		uint32_t* image;
		int imageWidth, imageHeight;
		int srcLeft, srcTop, srcRight, srcBottom;
	};

	struct RenderCommandParams {
		int color;  //USADO POR CLEAR Y PUT_PIXEL
		int x, y;	//USADO POR DRAW_spriteSheet Y PUT_PIXEL
		//TODO: quitar put_pixel?
		union {
			SpriteSheetData SpriteSheetData;
		};
	};

	struct RenderCommand {
		RenderCommandType type;
		RenderCommandParams params;
	};

private:
	std::thread* _thread;
	std::atomic<bool> _quitRequested;
	std::atomic<int> _pendingframes;
	ConcurrentQueue<RenderCommand> _concurrentQueue;

	void renderLoop();
	int clamp(int value, int min, int max);
	void drawRescaled(int x, int y, const RenderCommandParams& params, const int scale);

public:
	RendererThread();
	~RendererThread();

	void start(); //comienza la ejecucion de la hebra
	void stop(); //espera a que acabe (join) y delete thread
	void enqueueCommand(RenderCommand c); //llamado desde la logica. actualiza el numero de frames que quedan por pintar
	inline int getPendingFrames() { return _pendingframes; }; //al final del bucle de la logica se tiene que poner while(cuantosFramesMeQuedanPorPintar > numFrameBuffers);
	//para que espere si se adelanta la logica a la hebra de render para que no se pisen frames ni se llene la cola con muchos comandos
	//y exploten
};

