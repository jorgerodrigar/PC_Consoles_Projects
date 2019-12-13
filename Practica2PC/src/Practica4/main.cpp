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
#include <Logic/BackgroundDoor.h>
#include <iostream>

int main() {
	Platform::init();
	Renderer::init();
	Input::init();
	Resources::getInstance();	//Load de los recursos

	RendererThread rendererThread;
	
	int frame = 0;
	
	/*GameManager* gm;
	gm = GameManager::getInstance();
	gm->init(&rendererThread);*/

	BackgroundDoor bd;
	bd.init();
	bd.setX(832);
	Door door;
	door.init();
	door.setId(0);
	door.setX(bd.getCenterX());
	door.setInitialPosX(bd.getCenterX());
	door.setY(bd.getCenterY());
	Client client;
	client.init();
	client.setX(bd.getCenterX());
	client.setY(bd.getCenterY());
	Bandit bandit;
	bandit.init();
	bandit.setX(bd.getCenterX());
	bandit.setY(bd.getCenterY());

	door.addListener(&client);
	door.addListener(&bandit);

	InputData data;
	rendererThread.start();

	while (Platform::tick())
	{
		//Renderer::clear(0x000000);
		Input::tick();
		InputData data = Input::getUserInput();
		if (data.buttonsInfo.L1 == 1) {
			StartEventMessage m(START_EVENT, door.getId());
			door.receiveMessage(m);
		}
		else if (data.leftStick.x > 0.7f) {
			door.receiveMessage(ShootMessage(SHOOT, door.getId()));
		}
		/*gm->handleInput();
		gm->update(Platform::getDeltaTime()); 
		gm->render();*/
		double deltaTime = Platform::getDeltaTime();
		bd.update(deltaTime);
		door.update(deltaTime);
		client.update(deltaTime);
		bandit.update(deltaTime);

		bd.render(&rendererThread);
		bandit.render(&rendererThread);
		client.render(&rendererThread);
		door.render(&rendererThread);

		RendererThread::RenderCommand command;
		command.type = RendererThread::END_FRAME;
		rendererThread.enqueueCommand(command);
		
		while (rendererThread.getPendingFrames() >= Renderer::getNumBuffers()); //espera activa de la cpu
	}

	rendererThread.stop();

	//GameManager::release();
	Resources::release();
	Input::release();
	Renderer::release();
	Platform::release();
	
	return 0;
}