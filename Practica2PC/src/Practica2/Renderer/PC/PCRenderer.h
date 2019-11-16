#ifndef PCRENDERER_H_  
#define PCRENDERER_H_

class SDL_Window;
class SDL_Renderer;


class PCRenderer {

private:
	static SDL_Window* _window;
	static SDL_Renderer* _renderer;
	static int WIN_WIDTH, WIN_HEIGHT;
	static bool _initialized;
	static int windowType;
	PCRenderer();


public:
	static void init();
	static void release();

	static void clear(int color);
	static void putPixel(int x, int y, int color);
	static void present();

	static int const getWindowWidth();
	static int const getWindowHeight();
	static const unsigned int getNumBuffers();

	static int* hexToRGBA(int hexColor);
};

#endif 