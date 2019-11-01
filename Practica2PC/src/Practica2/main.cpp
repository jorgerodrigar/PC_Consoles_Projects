#include <Renderer/Renderer.h>
#include <Platform/Platform.h>
#include <Logic/Fire.h>

int main() {
	Platform::init();
	Renderer::init("Fuego");
	bool exit = false;

	Fire fire;
	fire.initFire();

	while (!exit)
	{
		exit = Platform::tick();
		//Renderer::clear(50);
		fire.simulateFire(LIGHT);
		fire.renderFire();
		Renderer::present();
	}

	Renderer::release();
	Platform::release();

	return 0;
}