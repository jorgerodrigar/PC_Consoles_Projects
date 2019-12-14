#include <Renderer/Renderer.h>
#include <Platform/Platform.h>
#include <Renderer/RendererThread.h>
#include <Input/Input.h>
#include <Input/InputData.h>
#include <Utils/Resources.h>
#include <Logic/GameManager.h>
#include <Logic/Door.h>
#include <Logic/Client.h>
#include <Logic/Bandit.h>
#include <iostream>

int main() {
	Platform::init();
	Renderer::init();
	Input::init();
	Resources::getInstance();	//Load de los recursos

	RendererThread rendererThread;
	
	int frame = 0;
	
	GameManager* gm;
	gm = GameManager::getInstance();
	gm->init(&rendererThread);

	rendererThread.start();

	while (Platform::tick())
	{
		//Renderer::clear(0x000000);
		Input::tick();

		gm->handleInput();
		gm->update(Platform::getDeltaTime()); 
		gm->render();

		RendererThread::RenderCommand command;
		command.type = RendererThread::END_FRAME;
		rendererThread.enqueueCommand(command);
		
		while (rendererThread.getPendingFrames() >= Renderer::getNumBuffers()); //espera activa de la cpu
	}

	rendererThread.stop();

	GameManager::release();
	Resources::release();
	Input::release();
	Renderer::release();
	Platform::release();
	
	return 0;
}