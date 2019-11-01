#include <Renderer/Renderer.h>
#include <Platform/Platform.h>

int main() {
	Platform* p = new Platform();
	p->init();
	while (true) {}
	return 0;
}