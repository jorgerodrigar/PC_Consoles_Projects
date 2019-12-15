#include "SpriteSheet.h"
#include <Renderer/RendererThread.h>
#include <Renderer/Renderer.h>
#include <iostream>

SpriteSheet::SpriteSheet(): _lastTime(0)
{
}

SpriteSheet::~SpriteSheet()
{
	_animations.clear();
}

void SpriteSheet::setImage(Resources::ImageId id)
{
	Resources* resources = Resources::getInstance();
	auto params = resources->getImage(id);
	_image = params.second;
	_width = params.first[0];
	_height = params.first[1];
}

void SpriteSheet::init(Resources::ImageId id, char rows, char cols, char frame)
{
	setImage(id);
	_rows = rows;
	_cols = cols;
	_currentFrame = frame;
	_isAnimated = false;
	_stopAnimating = false;

	int frameWidth = _width / _cols;
	int frameHeight = _height / _rows;
	int left = (frame % _cols)*frameWidth;
	int top = (frame / _cols)*frameHeight;

	_srcRect = Rect(left, top, frameWidth, frameHeight);
	_currentSrcRect = _srcRect;
}

//Dibuja la imagen completa en la posicion x, y
void SpriteSheet::draw(int x, int y, RendererThread * renderThread)
{
	sendDrawCommand(x, y, 0, 0, _width, _height, renderThread);
}

//Dibuja un trozo de la imagen en la posicion x, y 
void SpriteSheet::draw(int x, int y, int left, int top, int right, int bottom, RendererThread * renderThread)
{
	sendDrawCommand(x, y, left, top, right, bottom, renderThread);
}

void SpriteSheet::draw(int x, int y, char rows, char cols, char frame, RendererThread * renderThread)
{
	int left = _currentSrcRect.left + (frame % cols) * _srcRect.right;
	int top = _currentSrcRect.top + (frame / cols) * _srcRect.bottom;
	sendDrawCommand(x, y, left, top, _currentSrcRect.right, _currentSrcRect.bottom, renderThread);
}

bool SpriteSheet::update(double deltaTime)
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

void SpriteSheet::render(int x, int y, RendererThread* renderThread)
{
	draw(x, y, _rows, _cols, _currentFrame, renderThread);
}

void SpriteSheet::addAnim(std::string name, AnimInfo& animInfo)
{
	animInfo.name = name; //lo pongo aqui para no modificar todos los addAnim que no he hecho TODO: CAMBIARLO?
	_animations.insert({ name, animInfo });
}

void SpriteSheet::setAnim(std::string name)
{
	auto it = _animations.find(name);
	if (it != _animations.end()) {
		_currentAnim = it->second;
		_currentAnim.name = name;
		_currentFrame = _currentAnim.iniFrame;
		_isAnimated = true;
		_stopAnimating = false;
	}
}

void SpriteSheet::setFrame(char frame)
{
	_currentFrame = frame;
}

void SpriteSheet::setCurrentRect(Rect rect)
{
	_currentSrcRect = rect;
}

SpriteSheet::Rect SpriteSheet::getRect()
{
	return _srcRect;
}

void SpriteSheet::changeFrame()
{
	if (!_stopAnimating) {
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
}

//Crea y encola un comando DRAW_spriteSheet a la hebra de render
void SpriteSheet::sendDrawCommand(int x, int y, int left, int top, int right, int bottom, RendererThread * renderThread)
{
	RendererThread::RenderCommand command;
	command.type = RendererThread::DRAW_spriteSheet;
	command.params.x = x;
	command.params.y = y;
	command.params.SpriteSheetData.image = _image;
	command.params.SpriteSheetData.imageWidth = _width;
	command.params.SpriteSheetData.imageHeight = _height;
	command.params.SpriteSheetData.srcLeft = left;
	command.params.SpriteSheetData.srcTop = top;
	command.params.SpriteSheetData.srcRight = left + right;
	command.params.SpriteSheetData.srcBottom = top + bottom;

	renderThread->enqueueCommand(command);
}

int SpriteSheet::getWidth()
{
	return _width;
}

int SpriteSheet::getHeight()
{
	return _height;
}

int const SpriteSheet::getFrameWidth() const
{
	return _currentSrcRect.right;
}

int const SpriteSheet::getFrameHeight() const
{
	return _currentSrcRect.bottom;
}

bool const SpriteSheet::isAnimated() const
{
	return _isAnimated;
}

void SpriteSheet::stopAnimation()
{
	_stopAnimating = true;
}

void SpriteSheet::sourceInWidthBounds(float& x, int boundMin, int boundMax)
{
	if (x + (_srcRect.left) <= boundMin) {
		_currentSrcRect.left = _srcRect.left + ((boundMin - x));
		_currentSrcRect.right = _srcRect.right - _currentSrcRect.left;
		x = boundMin;
	}

	if ((x + (_srcRect.right) >= boundMax)) {
		_currentSrcRect.right = _currentSrcRect.left + ((boundMax - x));
	}
}

int const SpriteSheet::getCurrentFrame() const
{
	return _currentFrame;
}

std::string SpriteSheet::getCurrentAnimName() const
{
	return _currentAnim.name;
}
