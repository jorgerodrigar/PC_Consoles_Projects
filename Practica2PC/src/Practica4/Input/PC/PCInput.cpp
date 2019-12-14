#if defined(_WIN64) || defined(_WIN32)

#include "PCInput.h"
#include <limits.h>

InputListener PCInput::_inputListener;
InputData PCInput::_inputData;

void PCInput::init()
{
	_inputListener.init();
}

void PCInput::release()
{
}

void PCInput::tick()
{
	std::list<SDL_Event> eventList = _inputListener.getEvents();
	for (SDL_Event event : eventList) {
		if (event.type == SDL_KEYDOWN) {
			handleKeyDown(event);
		}
		else if (event.type == SDL_KEYUP) {
			handleKeyUp(event);
		}
		else if (event.type == SDL_CONTROLLERDEVICEADDED)
		{
			openController(event);
		}
		else if (event.type == SDL_CONTROLLERAXISMOTION) {
			handleAxisMotion(event);
		}
		else if (event.type == SDL_CONTROLLERBUTTONDOWN) {
			handleButtonDown(event);
		}
		else if (event.type == SDL_CONTROLLERBUTTONUP) {
			handleButtonUp(event);
		}
	}
}

InputData PCInput::getUserInput()
{
	return _inputData;
}

PCInput::PCInput()
{
}

PCInput::~PCInput()
{
}

// -------------------------------KEYBOARD INPUT------------------------------------
void PCInput::handleKeyDown(const SDL_Event& event)
{
	switch (event.key.keysym.sym)
	{
	case SDLK_o:
		_inputData.buttonsInfo.L1 = 1.0f;
		break;
	case SDLK_p:
		_inputData.buttonsInfo.R1 = 1.0f;
		break;
	case SDLK_1:
		_inputData.leftStick.x = -1.0f;    // izq
		_inputData.triggersInfo.R2 = 1.0f; // disparo
		_inputData.triggersInfo.L2 = 1.0f; // disparo
		break;
	case SDLK_2:
		_inputData.leftStick.x = 0.0f;     // centro
		_inputData.triggersInfo.R2 = 1.0f; // disparo
		_inputData.triggersInfo.L2 = 1.0f; // disparo
		break;
	case SDLK_3:
		_inputData.leftStick.x = 1.0f;     // der
		_inputData.triggersInfo.R2 = 1.0f; // disparo
		_inputData.triggersInfo.L2 = 1.0f; // disparo
		break;
	default:
		break;
	}
}

void PCInput::handleKeyUp(const SDL_Event& event)
{
	switch (event.key.keysym.sym)
	{
	case SDLK_o:
		_inputData.buttonsInfo.L1 = 0.0f;
		break;
	case SDLK_p:
		_inputData.buttonsInfo.R1 = 0.0f;
		break;
	case SDLK_1:
		_inputData.leftStick.x = 0.0f;    // izq
		_inputData.triggersInfo.R2 = 0.0f; // disparo
		break;
	case SDLK_2:
		_inputData.leftStick.x = 0.0f;     // centro
		_inputData.triggersInfo.R2 = 0.0f; // disparo
		break;
	case SDLK_3:
		_inputData.leftStick.x = 0.0f;     // der
		_inputData.triggersInfo.R2 = 0.0f; // disparo
		break;
	default:
		break;
	}
}

// -------------------------------CONTROLLER INPUT------------------------------------
void PCInput::openController(const SDL_Event & event)
{
	if (SDL_IsGameController(event.cdevice.which))
	{
		SDL_GameController* controller = SDL_GameControllerOpen(event.cdevice.which);
		SDL_Joystick* joystick = SDL_GameControllerGetJoystick(controller);
		SDL_JoystickID joyId = SDL_JoystickInstanceID(joystick);
	}
}

void PCInput::handleAxisMotion(const SDL_Event& event)
{
	switch (event.caxis.axis) {
	case SDL_CONTROLLER_AXIS_LEFTX:
		_inputData.leftStick.x = event.caxis.value / (float)SHRT_MAX;
		break;
	case SDL_CONTROLLER_AXIS_LEFTY:
		_inputData.leftStick.y = event.caxis.value / (float)SHRT_MAX;
		break;
	case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
		_inputData.triggersInfo.L2 = event.caxis.value / (float)SHRT_MAX;
		break;
	case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
		_inputData.triggersInfo.R2 = event.caxis.value / (float)SHRT_MAX;
		break;
	}
}

void PCInput::handleButtonDown(const SDL_Event& event)
{
	switch (event.cbutton.button) {
	case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
		_inputData.buttonsInfo.L1 = 1.0f;
		break;
	case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
		_inputData.buttonsInfo.R1 = 1.0f;
		break;
	case SDL_CONTROLLER_BUTTON_B:
		_inputData.buttonsInfo.CIRCLE = 1.0f;
		break;
	case SDL_CONTROLLER_BUTTON_A:
		_inputData.buttonsInfo.CROSS = 1.0f;
		break;
	}
}

void PCInput::handleButtonUp(const SDL_Event& event)
{
	switch (event.cbutton.button) {
	case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
		_inputData.buttonsInfo.L1 = 0.0f;
		break;
	case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
		_inputData.buttonsInfo.R1 = 0.0f;
		break;
	case SDL_CONTROLLER_BUTTON_B:
		_inputData.buttonsInfo.CIRCLE = 0.0f;
		break;
	case SDL_CONTROLLER_BUTTON_A:
		_inputData.buttonsInfo.CROSS = 0.0f;
		break;
	}
}

#endif
