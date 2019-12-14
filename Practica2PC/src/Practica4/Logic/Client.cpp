#include "Client.h"
#include <iostream>

Client::Client()
{
}


Client::~Client()
{
}

void Client::init()
{
	_sprite.init(Resources::cliente, 1, 3, 0);
	Sprite::AnimInfo idleAnimInfo(0, 0, 0, false);
	_sprite.addAnim("idle", idleAnimInfo);
	Sprite::AnimInfo dyingAnimInfo(0.2f, 1, 2, false);
	_sprite.addAnim("dying", dyingAnimInfo);

	setActive(false);
}

void Client::receiveMessage(const Message & message)
{
	switch (message.type)
	{
	case DOOR_OPENING: {
		const DoorOpeningMessage* doorOpening = static_cast<const DoorOpeningMessage*>(&message);
		if (doorOpening->id == _id)
		{
			_currentDoorId = doorOpening->doorId;
			setX(doorOpening->posX);
			setY(doorOpening->posY);
			openDoor();
		}
		break;
	}
	case DOOR_CLOSED: {
		if (getActive())
		{
			closeDoor();
		}
		break;
	}
	case SET_DIRTY:
		setDirty();
		break;
	case SHOOT:
		if (getActive()){
			getAShot();
		}
		break;
	default:
		break;
	}
}

void Client::reset()
{
	GameObject::reset();
	_sprite.setAnim("idle");
	setActive(false);
}

void Client::openDoor()
{
	setActive(true);
	_sprite.setAnim("idle");
	setDirty();
}

void Client::closeDoor()
{
	if (_sprite.getCurrentAnimName() != "dying") {
		DepositMessage m(DEPOSIT, _currentDoorId);
		sendMessage(m);
	}
	else {
		Message m(GAME_OVER);
		sendMessage(m);
	}

	setActive(false);
}

void Client::getAShot()
{
	if (_sprite.getCurrentAnimName() != "dying") {
		_sprite.setAnim("dying");
		setDirty();
	}
}