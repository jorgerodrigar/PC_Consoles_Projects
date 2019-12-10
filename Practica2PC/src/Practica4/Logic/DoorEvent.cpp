#include "DoorEvent.h"
#include <Utils/Resources.h>

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
	Sprite::AnimInfo openingAnimInfo(1, 0, 4, false);
	_door.addAnim("opening", openingAnimInfo);
	Sprite::AnimInfo closingAnimInfo(1, 4, 0, false);
	_door.addAnim("closing", closingAnimInfo);
	openDoor();
	_client.init(Resources::cliente, 1, 3, 0);

	_centerX = _sprite.getWidth()/2 - (_door.getWidth()/3)/5; //provisional xd
	_centerY = _sprite.getHeight() / 2 - (_door.getHeight() / 3);
}

void DoorEvent::render(RendererThread * renderThread)
{
	if (_hasChanged && _active) {
		_sprite.render(_x, _y, renderThread);
		_door.render(_x + _centerX, _y + _centerY, renderThread);
		//_client.render(_x + _centerX, _y + _centerY, renderThread);
		_hasChanged = false;
	}
}

void DoorEvent::forceRender(RendererThread * renderThread)
{
	if (_active) {
		_sprite.render(_x, _y, renderThread);
		_door.render(_x + _centerX, _y + _centerY, renderThread);
		//_client.render(_x + _centerX, _y + _centerY, renderThread);
		_hasChanged = false;
	}
}

void DoorEvent::update(double deltaTime)
{
	bool aux = _sprite.update(deltaTime) || _door.update(deltaTime) || _client.update(deltaTime);
	if (!_hasChanged)
		_hasChanged = aux;

}

bool DoorEvent::receiveMessage(const Message & message)
{
	return false;
}

void DoorEvent::openDoor()
{
	_door.setAnim("opening");
	_isClosed = false;
}

void DoorEvent::closeDoor()
{
	_door.setAnim("closing");
	_isClosed = true;
}

bool DoorEvent::isClosed()
{
	return _isClosed;
}
