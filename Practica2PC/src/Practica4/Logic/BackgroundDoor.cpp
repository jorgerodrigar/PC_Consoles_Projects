#include "BackgroundDoor.h"
#include <Renderer/Renderer.h>
#include <iostream>

BackgroundDoor::BackgroundDoor()
{
}


BackgroundDoor::~BackgroundDoor()
{
}

void BackgroundDoor::init()
{
	_sprite.init(Resources::marcoPuerta, 1, 1);

	setActive(true);
	_sprite.setVisible(true);
}

void BackgroundDoor::receiveMessage(const Message & message)
{
	switch (message.type)
	{
	default:
		break;
	}
}

int const BackgroundDoor::getCenterX() const
{
	return getX() + (32 * 2); //provisional xd
}

int const BackgroundDoor::getCenterY() const
{
	return getY() + (24 * 2);
}
