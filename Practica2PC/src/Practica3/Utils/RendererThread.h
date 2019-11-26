#pragma once
#include <thread>
#include <atomic>
#include <Utils/ConcurrentQueue.h>

class RendererThread
{
public:
	enum RenderCommandType { CLEAR, END_FRAME, PUT_PIXEL, DRAW_BACKGROUND, WRITE_RAIN };

	struct RenderCommandPutPixelParams {
		int x, y;
	};

	struct SimulationData {
		int* current;
		int* image;
		int* increments;
	};

	struct RenderCommandParams { 
		int color;
		//union { // TODO: HACER QUE SEA UNION
			RenderCommandPutPixelParams putPixelParams;
			SimulationData simulationData;
		//};
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
	void renderRain(int* current, int* image, int* increments);
	int clamp(int value, int min, int max);

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

