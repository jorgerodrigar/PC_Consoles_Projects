#include "GameObject.h"
#include <Utils/Resources.h>
#include <Renderer/Renderer.h>
#include <iostream>

GameObject::GameObject(): _x(0), _y(0), _active(true), _pendingFrames(Renderer::getNumBuffers())
{
}


GameObject::~GameObject()
{
}

void GameObject::init()
{
	_sprite.init(Resources::debug500x500, 2, 2);
	Sprite::AnimInfo animInfo(1, 0, 3, true);
	_sprite.addAnim("test", animInfo);
	_sprite.setAnim("test");
}

void GameObject::render(RendererThread* renderThread)
{
	if (_active && _pendingFrames >= 0) {
		_sprite.render(_x, _y, renderThread);
		_pendingFrames--;
	}
}

void GameObject::update(double deltaTime)
{
	bool aux = _sprite.update(deltaTime);
	if (aux) {
		_pendingFrames = Renderer::getNumBuffers();
	}
}

bool GameObject::getActive()
{
	return _active;
}

void GameObject::setActive(bool value)
{
	_active = value;
}

float GameObject::getX()
{
	return _x;
}

float GameObject::getY()
{
	return _y;
}

void GameObject::setX(float value)
{
	_x = value;
	_pendingFrames = Renderer::getNumBuffers();
}

void GameObject::setY(float value)
{
	_y = value;
	_pendingFrames = Renderer::getNumBuffers();
}
