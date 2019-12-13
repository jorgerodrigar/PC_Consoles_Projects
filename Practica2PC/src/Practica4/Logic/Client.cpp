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
	_sprite.init(Resources::cliente, 1, 3, 0, false);
	Sprite::AnimInfo idleAnimInfo(0, 0, 0, false);
	_sprite.addAnim("idle", idleAnimInfo);
	Sprite::AnimInfo dyingAnimInfo(0.8f, 1, 2, false);
	_sprite.addAnim("dying", dyingAnimInfo);
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

void Client::openDoor()
{
	setActive(true);
	_sprite.setVisible(true);
	_sprite.setAnim("idle");
	setDirty();
}

void Client::closeDoor()
{
	if (_sprite.getCurrentAnimName() != "dying") {
		DepositMessage m(DEPOSIT, _currentDoorId);
		sendMessage(m);
	}

	setActive(false);
	_sprite.setVisible(false);
}

void Client::getAShot()
{
	if (_sprite.getCurrentAnimName() != "dying") {
		GameOverMessage m(GAME_OVER);
		sendMessage(m);

		_sprite.setAnim("dying");
		setDirty();
	}
}