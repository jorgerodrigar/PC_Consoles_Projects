#pragma once
#include <thread>
#include <atomic>
#include <Utils/ConcurrentQueue.h>
#include <stdint.h>

class RendererThread
{
public:
	///enumerado de tipos de comandos que puede hacer la hebra de pintado
	enum RenderCommandType { CLEAR, END_FRAME, PUT_PIXEL, DRAW_SPRITESHEET };

	///contiene los datos necesarios para dibujar un sprite en pantalla (imagen, rectangulo fuente / destino)
	struct SpriteSheetData {
		uint32_t* image;
		int imageWidth, imageHeight;
		int srcLeft, srcTop, srcRight, srcBottom;
	};

	///parametros usados por los comandos de render thread
	struct RenderCommandParams {
		int color;  //USADO POR CLEAR Y PUT_PIXEL
		int x, y;	//USADO POR DRAW_spriteSheet Y PUT_PIXEL
		SpriteSheetData SpriteSheetData;
	};

	struct RenderCommand {
		RenderCommandType type;
		RenderCommandParams params;
	};

private:
	///factor de escala del juego
	const unsigned char SCALE_FACTOR = 2;

	std::thread* _thread;
	std::atomic<bool> _quitRequested;
	std::atomic<int> _pendingframes;
	ConcurrentQueue<RenderCommand> _concurrentQueue;

	///funcion ejecutada cuando se llama al start. Bucle de render que lee los comandos que le van tocando.
	void renderLoop();
	int clamp(int value, int min, int max);
	///dibuja SCALE_FACTOR * SCALE_FACTOR pixeles por cada pixel recibido
	void drawRescaled(int x, int y, const RenderCommandParams& params, const int scale);

public:
	RendererThread();
	~RendererThread();

	///comienza la ejecucion de la hebra
	void start();
	///espera a que acabe (join) y delete thread
	void stop(); 
	///llamado desde la logica. actualiza el numero de frames que quedan por pintar si el comando recibido es de tipo END_FRAME
	void enqueueCommand(RenderCommand c);
	///al final del bucle de la logica se tiene que poner while(cuantosFramesMeQuedanPorPintar > numFrameBuffers);
	///para que espere si se adelanta la logica a la hebra de render para que no se pisen frames ni se llene la cola con muchos comandos
	///y exploten
	inline int getPendingFrames() { return _pendingframes; }; 
};

