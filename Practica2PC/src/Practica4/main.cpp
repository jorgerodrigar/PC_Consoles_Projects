#include <Renderer/Renderer.h>
#include <Platform/Platform.h>
#include <Renderer/RendererThread.h>
#include <Utils/Resources.h>
#include <Logic/Sprite.h>
#include <iostream>

int main() {
	Platform::init();
	Renderer::init();
	Resources::getInstance();	//Load de los recursos

	RendererThread rendererThread;

	bool exit = false;
	int frame = 0;
	
	Sprite sprite;
	sprite.setImage(Resources::marcoPuerta);
	sprite.draw(400, 400, 30, 50, 150, 150, &rendererThread);

	rendererThread.start();
	while (!exit)
	{
		//Renderer::clear(0x000000);
		exit = Platform::tick();

		//std::cout << frame << std::endl;

		RendererThread::RenderCommand command;
		command.type = RendererThread::END_FRAME;
		rendererThread.enqueueCommand(command);

		frame++;

		while (rendererThread.getPendingFrames() >= Renderer::getNumBuffers()); //espera activa de la cpu
	}

	rendererThread.stop();

	Resources::release();
	Renderer::release();
	Platform::release();
	
	return 0;
}