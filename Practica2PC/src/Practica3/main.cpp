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
	int frame = 0;

	while (!exit)
	{
		//Renderer::clear(0x000000);
		exit = Platform::tick();

		if (frame == 0) {
			screenSim.startRandomWave();
			//frame = 0;
		}

		screenSim.simulateRain();

		screenSim.render();
		Renderer::present();

		frame++;
	}

	Renderer::release();
	Platform::release();

	return 0;
}