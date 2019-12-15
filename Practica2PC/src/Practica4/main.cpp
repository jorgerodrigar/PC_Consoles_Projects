#include <Renderer/Renderer.h>
#include <Platform/Platform.h>
#include <Renderer/RendererThread.h>
#include <Input/Input.h>
#include <Utils/Resources.h>
#include <Logic/GameManager.h>

int main() {
	Platform::Init();
	Renderer::Init();
	Input::Init();
	Resources::GetInstance();	//Load de los recursos

	RendererThread rendererThread;
	
	GameManager* gm;
	gm = GameManager::GetInstance();
	gm->init(&rendererThread);

	rendererThread.start();

	while (Platform::Tick())
	{
		//Renderer::clear(0x000000);
		Input::Tick();

		gm->handleInput();
		gm->update(Platform::GetDeltaTime()); 
		gm->render();

		RendererThread::RenderCommand command;
		command.type = RendererThread::END_FRAME;
		rendererThread.enqueueCommand(command);
		
		while (rendererThread.getPendingFrames() >= Renderer::GetNumBuffers()); //espera activa de la cpu
	}

	rendererThread.stop();

	GameManager::Release();
	Resources::Release();
	Input::Release();
	Renderer::Release();
	Platform::Release();
	
	return 0;
}