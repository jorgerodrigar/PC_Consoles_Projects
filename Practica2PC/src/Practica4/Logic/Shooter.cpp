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
	if (_active) {
		InputData inputData = Input::getUserInput();

		if (inputData.triggersInfo.R2 > _deadZone) {
			if (inputData.leftStick.x > _deadZone) {
				ShootMessage message = ShootMessage(SHOOT, 2);
				sendMessage(message);
			}
			else if (inputData.leftStick.x < -_deadZone) {
				ShootMessage message = ShootMessage(SHOOT, 0);
				sendMessage(message);
			}
			else {
				ShootMessage message = ShootMessage(SHOOT, 1);
				sendMessage(message);
			}
		}
	}
}
