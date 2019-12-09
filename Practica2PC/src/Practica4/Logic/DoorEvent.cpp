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
	_door.init(Resources::puertas, 1, 3, 0);
	_client.init(Resources::cliente, 1, 3, 0);
}

void DoorEvent::render(RendererThread * renderThread)
{
	if (_hasChanged && _active) {
		_sprite.render(_x, _y, renderThread);
		_door.render(_x, _y, renderThread);
		_client.render(_x, _y, renderThread);
		_hasChanged = false;
	}
}

void DoorEvent::forceRender(RendererThread * renderThread)
{
	if (_active) {
		_sprite.render(_x, _y, renderThread);
		_door.render(_x, _y, renderThread);
		_client.render(_x, _y, renderThread);
		_hasChanged = false;
	}
}
