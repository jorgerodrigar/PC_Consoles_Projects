#include "Shooter.h"
#include <Input/Input.h>
#include <Input/InputData.h>
#include <Utils/Message.h>

Shooter::Shooter()
{
}


Shooter::~Shooter()
{
}

void Shooter::init()
{
	_deadZone = 0.7f;
}

void Shooter::handleInput()
{
	InputData inputData = Input::getUserInput();

	if (inputData.leftStick.x > _deadZone) {
		ShootMessage message = ShootMessage(SHOOT, 1);
	}
	else if (inputData.leftStick.x < -_deadZone) {
		ShootMessage message = ShootMessage(SHOOT, -1);
	}
	else {
		ShootMessage message = ShootMessage(SHOOT, 0);
		sendMessage(message);
	}
}
