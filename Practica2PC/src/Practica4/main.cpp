#include <Renderer/Renderer.h>
#include <Platform/Platform.h>
#include <Renderer/RendererThread.h>
#include <Input/Input.h>
#include <Input/InputData.h>
#include <Utils/Resources.h>
#include <Logic/GameObject.h>
#include <iostream>

int main() {
	Platform::init();
	Renderer::init();
	Input::init();
	Resources::getInstance();	//Load de los recursos

	RendererThread rendererThread;
	
	bool exit = false;
	int frame = 0;
	
	GameObject go;
	go.init();

	for (int i = 0; i < Renderer::getNumBuffers(); i++) {
		go.forceRender(&rendererThread);

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

		go.update(Platform::getDeltaTime());                             
		go.render(&rendererThread);

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