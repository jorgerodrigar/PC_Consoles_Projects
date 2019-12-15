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
	_spriteSheet.init(Resources::cliente, 1, 3, 0);
	SpriteSheet::AnimInfo idleAnimInfo(0, 0, 0, false);
	_spriteSheet.addAnim("idle", idleAnimInfo);
	SpriteSheet::AnimInfo dyingAnimInfo(0.2f, 1, 2, false);
	_spriteSheet.addAnim("dying", dyingAnimInfo);

	setActive(false);
}

void Client::receiveMessage(const Message & message)
{
	switch (message.type)
	{
	case DOOR_OPENING: {
		const DoorOpeningMessage* doorOpening = static_cast<const DoorOpeningMessage*>(&message);
		if (doorOpening->characterId == _id)
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
	_spriteSheet.setAnim("idle");
	setActive(false);
}

void Client::openDoor()
{
	setActive(true);
	_spriteSheet.setAnim("idle");
	setDirty();
}

void Client::closeDoor()
{
	if (_spriteSheet.getCurrentAnimName() != "dying") {
		IDMessage m(DEPOSIT, _currentDoorId);
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
	if (_spriteSheet.getCurrentAnimName() != "dying") {
		_spriteSheet.setAnim("dying");
		setDirty();
	}
}