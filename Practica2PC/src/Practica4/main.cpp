#include <Renderer/Renderer.h>
#include <Platform/Platform.h>
#include <Renderer/RendererThread.h>
#include <Input/Input.h>
#include <Input/InputData.h>
#include <Utils/Resources.h>
#include <Logic/Sprite.h>
#include <iostream>

int main() {
	Platform::init();
	Renderer::init();
	Input::init();
	Resources::getInstance();	//Load de los recursos

	RendererThread rendererThread;
	
	bool exit = false;
	int frame = 0;
	
	Sprite sprite;
	sprite.setImage(Resources::puertas);
	for (int i = 0; i < Renderer::getNumBuffers(); i++) {
		sprite.draw(120, 0, 1, 3, 1, &rendererThread);

		RendererThread::RenderCommand command;
		command.type = RendererThread::END_FRAME;
		rendererThread.enqueueCommand(command);
	}

	InputData data;

	rendererThread.start();
	while (!exit)
	{
		//Renderer::clear(0x000000);
		exit = Platform::tick();
		Input::tick();
		data = Input::getUserInput();

		//std::cout << data.buttonsInfo.L1 << std::endl;

		RendererThread::RenderCommand command;
		command.type = RendererThread::END_FRAME;
		rendererThread.enqueueCommand(command);

		frame++;

		while (rendererThread.getPendingFrames() >= Renderer::getNumBuffers()); //espera activa de la cpu
	}

	rendererThread.stop();

	Resources::release();
	Input::release();
	Renderer::release();
	Platform::release();
	
	return 0;
}