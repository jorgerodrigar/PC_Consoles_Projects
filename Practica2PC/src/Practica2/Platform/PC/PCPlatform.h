#ifndef PCPLATFORM_H_  
#define PCPLATFORM_H_

class SDL_Window;
class SDL_Renderer;

class PCPlatform {
private:
	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;

	int _winWidth, _winHeight;

public:
	PCPlatform();
	~PCPlatform();
	void init();
	void release();

	void tick();
};

#endif 