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
	_sprite.init(Resources::ladron, 1, 5, 0, false);
	Sprite::AnimInfo idleAnimInfo(0, 0, 0, false);
	_sprite.addAnim("idle", idleAnimInfo);
	Sprite::AnimInfo aimingAnimInfo(0, 1, 1, false);
	_sprite.addAnim("aiming", aimingAnimInfo);
	Sprite::AnimInfo dyingAnimInfo(0.8f, 2, 3, false);
	_sprite.addAnim("dying", dyingAnimInfo);

	setActive(false);
	_sprite.setVisible(false);
}

bool Bandit::receiveMessage(const Message & message)
{
	switch (message.type)
	{
	case DOOR_OPENING: {
		const DoorOpeningMessage* doorOpening = static_cast<const DoorOpeningMessage*>(&message);
		if (doorOpening->id == _id)
		{
			_currentDoorId = doorOpening->doorId;
			openDoor();
			return true;
		}
		return false;
	}
	case DOOR_CLOSED: {
		if (getActive())
		{
			closeDoor();
			return true;
		}
		return false;
	}
	case SET_DIRTY:
		setDirty();
		return true;
	case SHOOT:
		if (getActive()) {
			getAShot();
			return true;
		}
		return false;
	default:
		return false;
	}
}

void Bandit::closeDoor()
{
	if (_sprite.getCurrentAnimName() != "dying" && _isAiming) {
		GameOverMessage m(GAME_OVER);
		sendMessage(m);
	}
}

void Bandit::getAShot()
{
	if (_sprite.getCurrentAnimName() != "dying") {
		if (!_isAiming) {
			GameOverMessage m(GAME_OVER);
			sendMessage(m);
		}
		else {
			std::cout << "GJ BRO" << std::endl;
		}
		_sprite.setAnim("dying");
		setDirty();
	}
}

void Bandit::openDoor()
{
	setActive(true);
	_sprite.setVisible(true);
	_sprite.setAnim("idle");

	randomAiming();

	setDirty();
}
