#include "Bandit.h"
#include <Renderer/Renderer.h>
#include <iostream>

void Bandit::randomAiming()
{
	unsigned char rnd = rand() % 100;
	_isAiming = false;

	if (rnd < 40) {
		_spriteSheet.setAnim("aiming");
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
	_spriteSheet.init(Resources::ladron, 1, 5, 0);
	SpriteSheet::AnimInfo idleAnimInfo(0, 0, 0, false);
	_spriteSheet.addAnim("idle", idleAnimInfo);
	SpriteSheet::AnimInfo aimingAnimInfo(0, 1, 1, false);
	_spriteSheet.addAnim("aiming", aimingAnimInfo);
	SpriteSheet::AnimInfo dyingAnimInfo(0.2f, 2, 3, false);
	_spriteSheet.addAnim("dying", dyingAnimInfo);

	setActive(false);
}

void Bandit::receiveMessage(const Message & message)
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
	_spriteSheet.setAnim("idle");
	_isAiming = false;
	setActive(false);
}

void Bandit::closeDoor()
{
	if (_spriteSheet.getCurrentAnimName() != "dying") {
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
	if (_spriteSheet.getCurrentAnimName() != "dying") {
		_spriteSheet.setAnim("dying");
		setDirty();
	}
}

void Bandit::openDoor()
{
	setActive(true);
	_spriteSheet.setAnim("idle");

	randomAiming();

	setDirty();
}
