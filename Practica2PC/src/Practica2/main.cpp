#include <Renderer/Renderer.h>
#include <Platform/Platform.h>
#include <Logic/Fire.h>
#include <Logic/Bars.h>
#include <iostream>

/*
 *[0, 500] --> encender
 *[501, 1000] --> simular
 *[1001, 1200] --> apagar
*/
int main() {
	Platform::init();
	Renderer::init("Fuego");
	bool exit = false;

	Bars bars = Bars(Renderer::getWindowHeight() - FIRE_HEIGHT);
	Fire fire;
	fire.initFire();
	int frame = 0;
	bars.renderBars(frame); //1 vez x buffer
	while (!exit)
	{
		//Renderer::clear(0x000000);
		exit = Platform::tick();

		bars.renderBarsWithDelta(frame);
		//TODO: CICLICO
		/*if(frame/200 == 0)
			fire.simulateFire(LIGHT);
		else if(frame/200 == 1)
			fire.simulateFire(SIMULATE);
		else if(frame/200 == 2)
			fire.simulateFire(EXTINGUISH);
		
		fire.renderFire();*/
		Renderer::present();
		frame++;
		std::cout << frame << std::endl;
	}

	Renderer::release();
	Platform::release();

	return 0;
}