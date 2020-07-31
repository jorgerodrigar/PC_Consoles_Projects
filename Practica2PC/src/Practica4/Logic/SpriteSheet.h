#pragma once
#include <stdint.h>
#include <Utils/Resources.h>
#include <map>
#include <string>

class RendererThread;

///almacena la imagen. Metodos para pintar trozos de la misma, incluir animaciones, etc.
class SpriteSheet {
public:
	SpriteSheet();
	~SpriteSheet();

	///informacion de las animaciones (nombre, rate, loop, etc)
	struct AnimInfo {
		std::string name;
		float rate;
		char iniFrame, endFrame;
		bool isLooped;
		char count;
		AnimInfo():rate(0), iniFrame(0), endFrame(0), isLooped(false), count(0) {};
		AnimInfo(float rate, char iniFrame, char endFrame, bool isLooped):rate(rate), iniFrame(iniFrame), endFrame(endFrame), isLooped(isLooped), count(0) {};
	};

	///rectangulo (left, top, right, bottom)
	struct Rect {
		int left, top;
		int right, bottom;
		Rect() :left(0), top(0), right(0), bottom(0) {};
		Rect(int left, int top, int right, int bottom) :left(left), top(top), right(right), bottom(bottom) {};
	};

	///pide a resources la imagen y establece las variables necesarias (cuanto mide un frame, left, right, top, bottom, etc). Por defecto pinta el primer frame
	void init(Resources::ImageId id, char rows, char cols, char frame = 0);

	///dibuja toda la imagen en (x, y)
	void draw(int x, int y, RendererThread* renderThread);
	///dibuja un rectangulo de la imagen en (x, y)
	void draw(int x, int y, int left, int top, int right, int bottom, RendererThread * renderThread);
	///dibuja un frame de la imagen en (x, y)
	void draw(int x, int y, char rows, char cols, char frame, RendererThread * renderThread);
	///modifica el source Rect para que no se pinte fuera de los bounds. Solo en el ejeX, la x pasa por referencia.
	void sourceInWidthBounds(float& x, int boundMin, int boundMax);

	///update de las aniamciones. Devuelve true si currentFrame se modifica (coherencia de buffers)
	bool update(double deltaTime);
	///llama draw para dibujar el frame actual
	void render(int x, int y, RendererThread* renderThread);

	///anyade una nueva animacion al map
	void addAnim(std::string name, AnimInfo& animInfo);
	///para la animacion actual
	void stopAnimation();

	int const getWidth() const;
	int const getHeight() const;
	int const getFrameWidth() const;
	int const getFrameHeight() const;
	int const getCurrentFrame() const;
	Rect const getRect() const;
	std::string const getCurrentAnimName() const;
	bool const isAnimated() const;

	void setCurrentRect(Rect rect);
	void setImage(Resources::ImageId id);
	void setAnim(std::string name);
	void setFrame(char frame);

private:
	uint32_t* _image;
	AnimInfo _currentAnim;
	Rect _srcRect;
	Rect _currentSrcRect;
	float _lastTime; 
	bool _isAnimated, _stopAnimating;
	int  _width, _height;
	char _rows, _cols,_currentFrame;

	std::map<std::string, AnimInfo> _animations;

	///actualiza un frame en la animacion
	void changeFrame();
	///encola un comando de DRAW_SPRITESHEET en la hebra de render
	void sendDrawCommand(int x, int y, int left, int top, int right, int bottom, RendererThread * renderThread);
};