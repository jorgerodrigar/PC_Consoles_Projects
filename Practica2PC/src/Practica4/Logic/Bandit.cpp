#include "Bandit.h"
#include <Renderer/Renderer.h>
#include <iostream>

void Bandit::randomAiming()
{
	unsigned char rnd = rand() % 100;
	_isAiming = false;

	if (rnd < 40) {
		_sprite.setAnim("aiming");
		_isAiming = true;
	}
}

Bandit::Bandit(): _isAiming(false)
{
}


Bandit::~Bandit()
{
}

void Bandit::init()
{
	_sprite.init(Resources::ladron, 1, 5, 0);
	Sprite::AnimInfo idleAnimInfo(0, 0, 0, false);
	_sprite.addAnim("idle", idleAnimInfo);
	Sprite::AnimInfo aimingAnimInfo(0, 1, 1, false);
	_sprite.addAnim("aiming", aimingAnimInfo);
	Sprite::AnimInfo dyingAnimInfo(0.2f, 2, 3, false);
	_sprite.addAnim("dying", dyingAnimInfo);

	setActive(false);
}

void Bandit::receiveMessage(const Message & message)
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
		if (getActive()) {
			getAShot();
		}
		break;
	default:
		break;
	}
}

void Bandit::reset()
{
	GameObject::reset();
	_sprite.setAnim("idle");
	_isAiming = false;
	setActive(false);
}

void Bandit::closeDoor()
{
	if (_sprite.getCurrentAnimName() != "dying") {
		if (_isAiming) {
			Message m(GAME_OVER);
			sendMessage(m);
		}
	}
	else if (!_isAiming) {
		Message m(GAME_OVER);
		sendMessage(m);
	}

	setActive(false);
}

void Bandit::getAShot()
{
	if (_sprite.getCurrentAnimName() != "dying") {
		_sprite.setAnim("dying");
		setDirty();
	}
}

void Bandit::openDoor()
{
	setActive(true);
	_sprite.setAnim("idle");

	randomAiming();

	setDirty();
}
