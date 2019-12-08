#include "GameObject.h"
#include <Utils/Resources.h>
#include <iostream>

GameObject::GameObject(): _x(0), _y(0), _active(true), _hasChanged(true)
{
}


GameObject::~GameObject()
{
}

void GameObject::init()
{
	_sprite.init(2, 2);
	_sprite.setImage(Resources::debug500x500);
	Sprite::AnimInfo animInfo(1, 0, 3, true);
	_sprite.addAnim("test", animInfo);
	_sprite.setAnim("test");
}

void GameObject::render(RendererThread* renderThread)
{
	if (_hasChanged && _active) {
		_sprite.render(_x, _y, renderThread);
		_hasChanged = false;
	}
}

void GameObject::forceRender(RendererThread* renderThread)
{
	if (_active) {
		_sprite.render(_x, _y, renderThread);
		_hasChanged = false;
	}
}

void GameObject::update(double deltaTime)
{
	bool aux = _sprite.update(deltaTime);
	if (!_hasChanged) 
		_hasChanged = aux;
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
	_hasChanged = true;
}

void GameObject::setY(float value)
{
	_y = value;
	_hasChanged = true;
}
