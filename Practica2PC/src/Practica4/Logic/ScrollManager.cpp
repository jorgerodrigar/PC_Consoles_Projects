#include "ScrollManager.h"
#include <Input/Input.h>

ScrollManager::ScrollManager()
{
}

ScrollManager::ScrollManager(float vel):_scrollingRight(false), _scrollingLeft(false), _vel(vel)
{
}

ScrollManager::~ScrollManager()
{
}

void ScrollManager::update()
{
	_inputData = Input::getUserInput();

	if (!_scrollingLeft && _inputData.buttonsInfo.R1) { // scroll de la camara hacia la derecha
		_scrollingRight = true;                         // (las puertas se moveran hacia la izquierda)
		sendMessage(ScrollMessage(LATERAL_SCROLL, _vel, -1));
	}
	else if (!_scrollingRight && _inputData.buttonsInfo.L1) { // scroll de la camara hacia la izquierda
		_scrollingLeft = true;                                // (las puertas se moveran hacia la derecha)
		sendMessage(ScrollMessage(LATERAL_SCROLL, _vel, 1));
	}

	else if (_scrollingRight && !_inputData.buttonsInfo.R1) {
		_scrollingRight = false;
	}
	else if (_scrollingLeft && !_inputData.buttonsInfo.L1) {
		_scrollingLeft = false;
	}
}
