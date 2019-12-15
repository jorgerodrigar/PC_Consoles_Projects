#ifndef PCRENDERER_H_  
#define PCRENDERER_H_

class SDL_Window;
class SDL_Renderer;

///recibe 4 componentes de color y los junta en un mismo "hexadecimal". Formato RGBA
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
	///crea e inicia la ventana y renderer de SDL
	static void Init();
	///libera la ventana y el renderer de SDL
	static void Release();

	///limpia la pantalla
	static void Clear(int color);
	///pone un pixel en la posicion (x, y) con el color enviado en pantalla
	static void PutPixel(int x, int y, int color);
	///vuelca el current display buffer en pantalla y hace swap de buffers si es necesario
	static void Present();

	static int const GetWindowWidth();
	static int const GetWindowHeight();
	static const unsigned int GetNumBuffers();

	static void HexToRGBA(int hexColor, int* rgba);
};

#endif 