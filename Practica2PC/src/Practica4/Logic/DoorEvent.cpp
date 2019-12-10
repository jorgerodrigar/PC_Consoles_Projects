#include "DoorEvent.h"
#include <Utils/Resources.h>
#include <iostream>

void DoorEvent::throwRandomEvent()
{
	unsigned int rnd = rand() % 2;
	if (rand) {
		_client.setVisible(true);
	}
	else {
		_bandit.setVisible(true);
	}
}

bool const DoorEvent::isClient() const
{
	return _client.getVisible();
}

DoorEvent::DoorEvent()
{
}

DoorEvent::~DoorEvent()
{
}

void DoorEvent::init()
{
	_sprite.init(Resources::marcoPuerta, 1, 1);
	_door.init(Resources::puertas, 1, 4, 0);
	Sprite::AnimInfo openingAnimInfo(1, 0, 3, false);
	_door.addAnim("opening", openingAnimInfo);
	Sprite::AnimInfo closingAnimInfo(1, 3, 0, false);
	_door.addAnim("closing", closingAnimInfo);

	_client.init(Resources::cliente, 1, 3, 0, false);
	_bandit.init(Resources::ladron, 1, 5, 0, false);

	openDoor();

	_centerX = _sprite.getWidth()/2 - (_door.getFrameWidth()/2); //provisional xd
	_centerY = _sprite.getHeight() / 2 - (_door.getFrameHeight()/5);
}

void DoorEvent::render(RendererThread * renderThread)
{
	if (_hasChanged && _active) {
		_sprite.render(_x, _y, renderThread);
		_door.render(_x + _centerX, _y + _centerY, renderThread);
		_client.render(_x + _centerX, _y + _centerY, renderThread);
		_bandit.render(_x + _centerX, _y + _centerY, renderThread);
		_hasChanged = false;
	}
}

void DoorEvent::forceRender(RendererThread * renderThread)
{
	if (_active) {
		_sprite.render(_x, _y, renderThread);
		_door.render(_x + _centerX, _y + _centerY, renderThread);
		_client.render(_x + _centerX, _y + _centerY, renderThread);
		_bandit.render(_x + _centerX, _y + _centerY, renderThread);
		_hasChanged = false;
	}
}

void DoorEvent::update(double deltaTime)
{
	bool aux = _sprite.update(deltaTime) || _door.update(deltaTime) || _client.update(deltaTime);
	if (!_hasChanged)
		_hasChanged = aux;
	//setX(getX() + (deltaTime * 100));
}

bool DoorEvent::receiveMessage(const Message & message)
{
	return false;
}

void DoorEvent::openDoor()
{
	_door.setAnim("opening");
	throwRandomEvent();
	_isClosed = false;
}

void DoorEvent::closeDoor()
{
	_door.setAnim("closing");
	_bandit.setVisible(false);
	_client.setVisible(false);
	_isClosed = true;
}

bool const DoorEvent::isClosed() const
{
	return _isClosed;
}
