#include <Renderer/Renderer.h>
#include <Platform/Platform.h>
#include <Renderer/RendererThread.h>
#include <Input/Input.h>
#include <Input/InputData.h>
#include <Utils/Resources.h>
#include <Logic/GameObject.h>
#include <Logic/DoorEvent.h>
#include <iostream>

int main() {
	Platform::init();
	Renderer::init();
	Input::init();
	Resources::getInstance();	//Load de los recursos

	RendererThread rendererThread;
	
	int frame = 0;
	
	DoorEvent go;
	go.init();
	go.setId(0);

	for (int i = 0; i < Renderer::getNumBuffers(); i++) {
		go.forceRender(&rendererThread);

		RendererThread::RenderCommand command;
		command.type = RendererThread::END_FRAME;
		rendererThread.enqueueCommand(command);
	}
	bool kk = false;
	InputData data;
	rendererThread.start();
		bool kk1 = true;
	while (Platform::tick())
	{
		//Renderer::clear(0x000000);
		Input::tick();

		data = Input::getUserInput();
		if (data.buttonsInfo.L1 == 1) {
			kk = false;
			if(go.isClosed())
				go.startRandomEvent();
		}
		/*else if (data.leftStick.x > 0) {
			if (!go.isClosed() && kk1) {
				go.receiveMessage(ShootMessage(MessageType::SHOOT, 0));
				kk1 = false;
			}
		}*/
		else kk = true;

		/*if (go.isClosed() && !kk1)
			kk1 = true;*/

		if (kk)
			frame--;
		else
			frame++;
		//go.setX(frame * 10);
		//std::cout << data.buttonsInfo.L1 << std::endl;
		/*if(!kk)
			go.setX(go.getX() + Platform::getDeltaTime()*50);
		else
			go.setX(go.getX() - Platform::getDeltaTime()*50);*/
		go.update(Platform::getDeltaTime()); 
		go.render(&rendererThread);

		RendererThread::RenderCommand command;
		command.type = RendererThread::END_FRAME;
		rendererThread.enqueueCommand(command);
		
		while (rendererThread.getPendingFrames() >= Renderer::getNumBuffers()); //espera activa de la cpu
	}

	rendererThread.stop();

	Resources::release();
	Input::release();
	Renderer::release();
	Platform::release();
	
	return 0;
}