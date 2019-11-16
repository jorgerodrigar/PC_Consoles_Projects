#include <Renderer/Renderer.h>
#include <Platform/Platform.h>
#include <Logic/ScreenSimulation.h>
#include <iostream>

int main() {
	Platform::init();
	Renderer::init();

	ScreenSimulation screenSim;
	screenSim.init("..\\..\\files\\fdi.rgba");
	for (int fb = 0; fb < Renderer::getNumBuffers(); fb++) {
		screenSim.drawBackground();
		Renderer::present();
	}

	bool exit = false;

	while (!exit)
	{
		//Renderer::clear(0x000000);
		exit = Platform::tick();
		screenSim.render();
		Renderer::present();
	}

	Renderer::release();
	Platform::release();

	return 0;
}