#ifndef PCRENDERER_H_  
#define PCRENDERER_H_

class SDL_Window;
class SDL_Renderer;

#define RGBA(r, g, b, a) ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8) + ((a & 0xff))

class PCRenderer {

private:
	static SDL_Window* _window;
	static SDL_Renderer* _renderer;
	static int WIN_WIDTH, WIN_HEIGHT;
	static bool _initialized;
	static int windowType;
	PCRenderer();


public:
	static void Init();
	static void Release();

	static void Clear(int color);
	static void PutPixel(int x, int y, int color);
	static void Present();

	static int const GetWindowWidth();
	static int const GetWindowHeight();
	static const unsigned int GetNumBuffers();

	static void HexToRGBA(int hexColor, int* rgba);
};

#endif 