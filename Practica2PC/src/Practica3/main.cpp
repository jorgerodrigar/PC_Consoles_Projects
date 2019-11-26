#include <Renderer/Renderer.h>
#include <Platform/Platform.h>
#include <Logic/ScreenSimulation.h>
#include <Utils/user_malloc.h>
#include <Utils/RendererThread.h>
#include <iostream>

//"..\\..\\files\\fdi.rgba"
//"/app0/fdi.rgba"

int main() {
	Platform::init();
	Renderer::init();

	RendererThread rendererThread;
	ScreenSimulation screenSim;

	screenSim.init("..\\..\\files\\fdi.rgba", &rendererThread);
	for (int fb = 0; fb < Renderer::getNumBuffers(); fb++) {
		screenSim.drawBackground();
		RendererThread::RenderCommand command;
		command.type = RendererThread::END_FRAME;
		rendererThread.enqueueCommand(command);
	}

	bool exit = false;
	int frame = 0;

	rendererThread.start();
	while (!exit)
	{
		//Renderer::clear(0x000000);
		exit = Platform::tick();

		if (frame == 5) {
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