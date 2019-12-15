#if defined(_WIN64) || defined(_WIN32)

#include "PCInput.h"
#include <limits.h>
#include <Input/InputData.h>
#include <Input/PC/InputListener.h>

InputData* PCInput::_inputData = nullptr;
InputListener* PCInput::_inputListener = nullptr;

void PCInput::Init()
{
	_inputData = new InputData();
	_inputListener = new InputListener();
	_inputListener->init();
}

void PCInput::Release()
{
	delete _inputData; _inputData = nullptr;
	delete _inputListener; _inputListener = nullptr;
}

void PCInput::Tick()
{
	std::list<SDL_Event> eventList = _inputListener->getEvents();
	for (SDL_Event event : eventList) {
		if (event.type == SDL_KEYDOWN) {
			HandleKeyDown(event);
		}
		else if (event.type == SDL_KEYUP) {
			HandleKeyUp(event);
		}
		else if (event.type == SDL_CONTROLLERDEVICEADDED)
		{
			OpenController(event);
		}
		else if (event.type == SDL_CONTROLLERAXISMOTION) {
			HandleAxisMotion(event);
		}
		else if (event.type == SDL_CONTROLLERBUTTONDOWN) {
			HandleButtonDown(event);
		}
		else if (event.type == SDL_CONTROLLERBUTTONUP) {
			HandleButtonUp(event);
		}
	}
}

InputData* PCInput::GetUserInput()
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
void PCInput::HandleKeyDown(const SDL_Event& event)
{
	switch (event.key.keysym.sym)
	{
	case SDLK_o:
		_inputData->buttonsInfo.L1 = 1.0f;
		break;
	case SDLK_p:
		_inputData->buttonsInfo.R1 = 1.0f;
		break;
	case SDLK_1:
		_inputData->leftStick.x = -1.0f;    // izq
		_inputData->triggersInfo.R2 = 1.0f; // disparo
		_inputData->triggersInfo.L2 = 1.0f; // disparo
		break;
	case SDLK_2:
		_inputData->leftStick.x = 0.0f;     // centro
		_inputData->triggersInfo.R2 = 1.0f; // disparo
		_inputData->triggersInfo.L2 = 1.0f; // disparo
		break;
	case SDLK_3:
		_inputData->leftStick.x = 1.0f;     // der
		_inputData->triggersInfo.R2 = 1.0f; // disparo
		_inputData->triggersInfo.L2 = 1.0f; // disparo
		break;
	default:
		break;
	}
}

void PCInput::HandleKeyUp(const SDL_Event& event)
{
	switch (event.key.keysym.sym)
	{
	case SDLK_o:
		_inputData->buttonsInfo.L1 = 0.0f;
		break;
	case SDLK_p:
		_inputData->buttonsInfo.R1 = 0.0f;
		break;
	case SDLK_1:
		_inputData->leftStick.x = 0.0f;     // izq
		_inputData->triggersInfo.R2 = 0.0f; // disparo
		_inputData->triggersInfo.L2 = 0.0f; // disparo
		break;
	case SDLK_2:
		_inputData->leftStick.x = 0.0f;     // centro
		_inputData->triggersInfo.R2 = 0.0f; // disparo
		_inputData->triggersInfo.L2 = 0.0f; // disparo
		break;
	case SDLK_3:
		_inputData->leftStick.x = 0.0f;     // der
		_inputData->triggersInfo.R2 = 0.0f; // disparo
		_inputData->triggersInfo.L2 = 0.0f; // disparo
		break;
	default:
		break;
	}
}

// -------------------------------CONTROLLER INPUT------------------------------------
void PCInput::OpenController(const SDL_Event & event)
{
	if (SDL_IsGameController(event.cdevice.which))
	{
		SDL_GameController* controller = SDL_GameControllerOpen(event.cdevice.which);
		SDL_Joystick* joystick = SDL_GameControllerGetJoystick(controller);
		SDL_JoystickID joyId = SDL_JoystickInstanceID(joystick);
	}
}

void PCInput::HandleAxisMotion(const SDL_Event& event)
{
	switch (event.caxis.axis) {
	case SDL_CONTROLLER_AXIS_LEFTX:
		_inputData->leftStick.x = event.caxis.value / (float)SHRT_MAX;
		break;
	case SDL_CONTROLLER_AXIS_LEFTY:
		_inputData->leftStick.y = event.caxis.value / (float)SHRT_MAX;
		break;
	case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
		_inputData->triggersInfo.L2 = event.caxis.value / (float)SHRT_MAX;
		break;
	case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
		_inputData->triggersInfo.R2 = event.caxis.value / (float)SHRT_MAX;
		break;
	}
}

void PCInput::HandleButtonDown(const SDL_Event& event)
{
	switch (event.cbutton.button) {
	case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
		_inputData->buttonsInfo.L1 = 1.0f;
		break;
	case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
		_inputData->buttonsInfo.R1 = 1.0f;
		break;
	case SDL_CONTROLLER_BUTTON_B:
		_inputData->buttonsInfo.CIRCLE = 1.0f;
		break;
	case SDL_CONTROLLER_BUTTON_A:
		_inputData->buttonsInfo.CROSS = 1.0f;
		break;
	}
}

void PCInput::HandleButtonUp(const SDL_Event& event)
{
	switch (event.cbutton.button) {
	case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
		_inputData->buttonsInfo.L1 = 0.0f;
		break;
	case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
		_inputData->buttonsInfo.R1 = 0.0f;
		break;
	case SDL_CONTROLLER_BUTTON_B:
		_inputData->buttonsInfo.CIRCLE = 0.0f;
		break;
	case SDL_CONTROLLER_BUTTON_A:
		_inputData->buttonsInfo.CROSS = 0.0f;
		break;
	}
}

#endif
