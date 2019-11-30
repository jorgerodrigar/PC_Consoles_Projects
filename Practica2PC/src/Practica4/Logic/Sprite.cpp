#include "Sprite.h"
#include <Renderer/RendererThread.h>

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

void Sprite::setImage(Resources::ImageId id)
{
	Resources* resources = Resources::getInstance();
	auto params = resources->getImage(id);
	_image = params.second;
	_width = params.first[0];
	_height = params.first[1];
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
