#include <Renderer/Renderer.h>
#include <Platform/Platform.h>
#include <Logic/Fire.h>
#include <Logic/Bars.h>
#include <iostream>


/*
 *[0, 200] --> encender
 *(200, 400] --> simular
 *(400, 600] --> apagar
*/
#define FIREFRAMES 200

int main() {
	Platform::init();
	Renderer::init();

	bool exit = false;
	// establecemos altura para que no colisione con el fuego
	Bars bars = Bars(Renderer::getWindowHeight() - FIRE_HEIGHT, 0);
	Fire fire;
	fire.initFire();

	int frame = 0;

	for (int i = 0; i < Renderer::getNumBuffers(); i++) {
		bars.renderBars(i); //1 vez x buffer
		Renderer::present(); //swap buffers
	}

	while (!exit)
	{
		//Renderer::clear(0x000000);
		exit = Platform::tick();

		//BARRAS
		bars.renderBarsWithDelta();

		//FUEGO
		if (frame / FIREFRAMES == 0)
			fire.simulateFire(LIGHT);
		else if (frame / FIREFRAMES == 1)
			fire.simulateFire(SIMULATE);
		else if (frame / FIREFRAMES == 2)
			fire.simulateFire(EXTINGUISH);
		else
			frame = -1;
		fire.renderFire();

		Renderer::present();

		frame++;
	}

	Renderer::release();
	Platform::release();

	return 0;
}