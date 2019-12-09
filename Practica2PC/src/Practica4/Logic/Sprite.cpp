#include "Sprite.h"
#include <Renderer/RendererThread.h>
#include <Renderer/Renderer.h>

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
	_animations.clear();
}

void Sprite::setImage(Resources::ImageId id)
{
	Resources* resources = Resources::getInstance();
	auto params = resources->getImage(id);
	_image = params.second;
	_width = params.first[0];
	_height = params.first[1];
}

void Sprite::init(Resources::ImageId id, char rows, char cols, char frame)
{
	setImage(id);
	_rows = rows;
	_cols = cols;
	_currentFrame = frame;

	int frameWidth = _width / cols;
	int frameHeight = _height / rows;
	int left = (frame % cols)*frameWidth;
	int top = (frame / cols)*frameHeight;

	_srcRect = Rect(left, top, frameWidth, frameHeight);
	_currentSrcRect = _srcRect;
}

//Dibuja la imagen completa en la posicion x, y
void Sprite::draw(int x, int y, RendererThread * renderThread)
{
	sendDrawCommand(x, y, 0, 0, _width, _height, renderThread);
}

//Dibuja un trozo de la imagen en la posicion x, y 
void Sprite::draw(int x, int y, int left, int top, int right, int bottom, RendererThread * renderThread)
{
	sendDrawCommand(x, y, left, top, right, bottom, renderThread);
}

void Sprite::draw(int x, int y, char rows, char cols, char frame, RendererThread * renderThread)
{
	sendDrawCommand(x, y, _currentSrcRect.left, _currentSrcRect.top, _currentSrcRect.right, _currentSrcRect.bottom, renderThread);
}

bool Sprite::update(double deltaTime)
{
	if (_isAnimated) {
		_lastTime += deltaTime;

		if (_lastTime >= _currentAnim.rate) {
			changeFrame();
			_lastTime = 0;
			return true;
		}
	}
	return false;
}

void Sprite::render(int x, int y, RendererThread* renderThread)
{
	sourceInWidthBounds(x, 0, Renderer::getWindowWidth());
	draw(x, y, _rows, _cols, _currentFrame, renderThread);
}

void Sprite::addAnim(std::string name, AnimInfo& animInfo)
{
	_animations.insert({ name, animInfo });
}

void Sprite::setAnim(std::string name)
{
	auto it = _animations.find(name);
	if (it != _animations.end()) {
		_currentAnim = it->second;
		_currentFrame *= _currentAnim.iniFrame;
		_isAnimated = true;
	}
}

void Sprite::setCurrentRect(Rect rect)
{
	_currentSrcRect = rect;
}

Sprite::Rect Sprite::getRect()
{
	return _srcRect;
}

void Sprite::changeFrame()
{
	_currentFrame++;
	
	if (_currentFrame > _currentAnim.endFrame) {
		if (_currentAnim.isLooped) 
			_currentFrame = _currentAnim.iniFrame;
		else {
			_isAnimated = false;
			_currentFrame = _currentAnim.endFrame;
		}
	}
}

//Crea y encola un comando DRAW_SPRITE a la hebra de render
void Sprite::sendDrawCommand(int x, int y, int left, int top, int right, int bottom, RendererThread * renderThread)
{
	RendererThread::RenderCommand command;
	command.type = RendererThread::DRAW_SPRITE;
	command.params.x = x;
	command.params.y = y;
	command.params.spriteData.image = _image;
	command.params.spriteData.imageWidth = _width;
	command.params.spriteData.imageHeight = _height;
	command.params.spriteData.srcLeft = left;
	command.params.spriteData.srcTop = top;
	command.params.spriteData.srcRight = left + right;
	command.params.spriteData.srcBottom = top + bottom;

	renderThread->enqueueCommand(command);
}

int Sprite::getWidth()
{
	return _width;
}

int Sprite::getHeight()
{
	return _height;
}

void Sprite::sourceInWidthBounds(int x, int boundMin, int boundMax)
{
	if (x < boundMin) {
		_currentSrcRect.left = _srcRect.left + (boundMin - x);
		_currentSrcRect.right = _srcRect.right - (boundMin - x);
		x = 100;
	}

	if ((x + (_currentSrcRect.right * SCALE_FACTOR) >= boundMax)) {
		_currentSrcRect.right -= ((x + (_currentSrcRect.right * SCALE_FACTOR)) - (boundMax)) / SCALE_FACTOR;
	}
}