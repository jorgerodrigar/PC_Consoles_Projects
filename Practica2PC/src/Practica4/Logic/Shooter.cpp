#include "Shooter.h"
#include <Input/Input.h>
#include <Input/InputData.h>
#include <Logic/Message.h>
#include <Logic/GameManager.h>

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
	if (_active && !GameManager::GetInstance()->allDoorsClosed()) {
		InputData* inputData = Input::GetUserInput();

		if (inputData->triggersInfo.R2 > _deadZone || inputData->triggersInfo.L2 > _deadZone) {
			if (inputData->leftStick.x > _deadZone) {
				IDMessage message = IDMessage(SHOOT, 2);
				sendMessage(message);
			}
			else if (inputData->leftStick.x < -_deadZone) {
				IDMessage message = IDMessage(SHOOT, 0);
				sendMessage(message);
			}
			else {
				IDMessage message = IDMessage(SHOOT, 1);
				sendMessage(message);
			}
		}
	}
}

void Shooter::render(RendererThread * renderThread)
{
}
