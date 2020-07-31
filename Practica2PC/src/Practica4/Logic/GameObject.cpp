#include "GameObject.h"
#include <Utils/Resources.h>
#include <Renderer/Renderer.h>
#include <iostream>

void GameObject::setDirty()
{
	_pendingFrames = Renderer::GetNumBuffers();
}

GameObject::GameObject(): _x(0), _y(0), _active(true), _pendingFrames(Renderer::GetNumBuffers())
{
}


GameObject::~GameObject()
{
}

void GameObject::init()
{
	_spriteSheet.init(Resources::debug500x500, 2, 2);
	SpriteSheet::AnimInfo animInfo(1, 0, 3, true);
	_spriteSheet.addAnim("test", animInfo);
	_spriteSheet.setAnim("test");
}

void GameObject::render(RendererThread* renderThread)
{
	if (_active && _pendingFrames >= 0) {
		_spriteSheet.render(_x, _y, renderThread);
		_pendingFrames--;
	}
}

void GameObject::update(double deltaTime)
{
	if (_active && _spriteSheet.update(deltaTime)) {
		setDirty();
	}
}

void GameObject::reset()
{
	_active = true;
	setDirty();
}

bool const GameObject::getActive() const
{
	return _active;
}

void GameObject::setActive(bool value)
{
	_active = value;
}

float const GameObject::getX() const
{
	return _x;
}

float const GameObject::getY() const
{
	return _y;
}

void GameObject::setX(float value)
{
	_x = value;
	setDirty();
}

void GameObject::setY(float value)
{
	_y = value;
	setDirty();
}
