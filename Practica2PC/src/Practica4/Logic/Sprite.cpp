#include "Sprite.h"
#include <Renderer/RendererThread.h>
#include <Renderer/Renderer.h>
#include <iostream>

Sprite::Sprite(): _lastTime(0)
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

void Sprite::init(Resources::ImageId id, char rows, char cols, char frame, bool visible)
{
	setImage(id);
	_rows = rows;
	_cols = cols;
	_currentFrame = frame;
	_isAnimated = false;
	_isVisible = visible;

	int frameWidth = _width / _cols;
	int frameHeight = _height / _rows;
	int left = (frame % _cols)*frameWidth;
	int top = (frame / _cols)*frameHeight;

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
	int left = _currentSrcRect.left + (frame % cols) * _srcRect.right;
	int top = _currentSrcRect.top + (frame / cols) * _srcRect.bottom;
	sendDrawCommand(x, y, left, top, _currentSrcRect.right, _currentSrcRect.bottom, renderThread);
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
	if (_isVisible) {  // PROVISIONAL
		sourceInWidthBounds(x, 32.0f*2.0f, Renderer::getWindowWidth() - (32.0f*2.0f));
		draw(x, y, _rows, _cols, _currentFrame, renderThread);
	}
}

void Sprite::addAnim(std::string name, AnimInfo& animInfo)
{
	animInfo.name = name; //lo pongo aqui para no modificar todos los addAnim que no he hecho TODO: CAMBIARLO?
	_animations.insert({ name, animInfo });
}

void Sprite::setAnim(std::string name)
{
	auto it = _animations.find(name);
	if (it != _animations.end()) {
		_currentAnim = it->second;
		_currentAnim.name = name;
		_currentFrame = _currentAnim.iniFrame;
		_isAnimated = true;
	}
}

void Sprite::setFrame(char frame)
{
	_currentFrame = frame;
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
	if (_currentAnim.iniFrame > _currentAnim.endFrame)
		_currentFrame--;
	else
		_currentFrame++;
	
	_currentAnim.count++;

	if (_currentAnim.count > abs(_currentAnim.endFrame - _currentAnim.iniFrame)) {
		if (_currentAnim.isLooped) {
			_currentFrame = _currentAnim.iniFrame;
		}
		else {
			_isAnimated = false;
			_currentFrame = _currentAnim.endFrame;
		}
		_currentAnim.count = 0;
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

int const Sprite::getFrameWidth() const
{
	return _currentSrcRect.right;
}

int const Sprite::getFrameHeight() const
{
	return _currentSrcRect.bottom;
}

bool const Sprite::getVisible() const
{
	return _isVisible;
}

bool const Sprite::isAnimated() const
{
	return _isAnimated;
}

void const Sprite::setVisible(bool value)
{
	_isVisible = value;
}

void Sprite::sourceInWidthBounds(int& x, int boundMin, int boundMax)
{
	if (x + (_srcRect.left * SCALE_FACTOR) <= boundMin) {
		_currentSrcRect.left = _srcRect.left + ((boundMin - x) / SCALE_FACTOR);
		_currentSrcRect.right = _srcRect.right - _currentSrcRect.left;
		x = boundMin;
	}

	if ((x + (_srcRect.right * SCALE_FACTOR) >= boundMax)) {
		_currentSrcRect.right = _currentSrcRect.left + ((boundMax - x) / SCALE_FACTOR);
	}
}

int const Sprite::getCurrentFrame() const
{
	return _currentFrame;
}

std::string Sprite::getCurrentAnimName() const
{
	return _currentAnim.name;
}
