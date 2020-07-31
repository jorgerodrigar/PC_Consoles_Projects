#include <Renderer/Renderer.h>
#include <Platform/Platform.h>
#include <Logic/ScreenSimulation.h>
#include <Utils/RendererThread.h>
#include <iostream>

int main() {
	Platform::init();
	Renderer::init();

	RendererThread rendererThread;

	// iniciamos la simulacion leyendo el archivo del fondo
	ScreenSimulation screenSim;
	std::string filePath = HEADER_PATH; // obtiene la ruta especifica de cada plataforma
	filePath.append("assets/fdi.rgba");
	screenSim.init(filePath.c_str(), &rendererThread);

	// dibujamos el fondo una sola vez por buffer
	for (int fb = 0; fb < Renderer::getNumBuffers(); fb++) {
		screenSim.drawBackground();
		RendererThread::RenderCommand command;
		command.type = RendererThread::END_FRAME;
		rendererThread.enqueueCommand(command);
	}

	bool exit = false;
	int frame = 0;

	rendererThread.start(); // se lanza la hebra de renderizado
	while (!exit)
	{
		//Renderer::clear(0x000000);
		exit = Platform::tick();

		if (frame == 10) { // cada 10 frames se lanza una gota aleatoria
			screenSim.startRandomWave();
			frame = 0;
		}

		//std::cout << frame << std::endl;

		screenSim.simulateRain();

		RendererThread::RenderCommand command;
		command.type = RendererThread::END_FRAME;
		rendererThread.enqueueCommand(command);

		screenSim.swap();

		frame++;

		while (rendererThread.getPendingFrames() >= Renderer::getNumBuffers()); //espera activa de la cpu
	}

	rendererThread.stop();
	Renderer::release();
	Platform::release();

	return 0;
}