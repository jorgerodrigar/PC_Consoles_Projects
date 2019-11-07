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

	PCRenderer();


public:
	static void init(const char* winName, int winWidth = 800, int winHeight = 600);
	static void release();

	static void clear(int color);
	static void putPixel(int x, int y, int color);
	static void present();

	static int const getWindowWidth();
	static int const getWindowHeight();
	static const unsigned int getNumBuffers();

	static int* hexToBGR(int hexColor);
};

#endif 