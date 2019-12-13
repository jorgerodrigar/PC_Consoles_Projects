#include "ScrollManager.h"
#include <Input/Input.h>
#include <Renderer/Renderer.h>

void ScrollManager::setNextTargetPositions()
{
	for (int i = 0; i < 4; i++) {
		if (isOutsideBounds(_actualPositions[i])) {
			if (_dir > 0)
				_actualPositions[i] = ((32.0f*2.0f) - (192.0f*2.0f)); // PROVISIONAL
			else if (_dir < 0)
				_actualPositions[i] = (Renderer::getWindowWidth() - (32.0f*2.0f)); // PROVISIONAL
		}
		_targetPositions[i] = _actualPositions[i] + (_dir * 192 * 2);// PROVISIONAL
	}

	//send msg a puertas -> invisibles
}

bool ScrollManager::isOutsideBounds(float x) // PROVISIONAL
{
	return (x >= Renderer::getWindowWidth() - 32.0f*2.0f || x + (192 * 2) <= 32.0f*2.0f);
}

ScrollManager::ScrollManager()
{
}

ScrollManager::ScrollManager(float vel):_scrollingRight(false), _scrollingLeft(false), _vel(vel)
{
}

ScrollManager::~ScrollManager()
{
}

void ScrollManager::init()
{
	_sprite.init(Resources::marcoPuerta, 1, 1);
	_dir = 0;
	_x = 32 * 2; // PROVISIONAL
	_y = 48 * 2;

	for (int i = 0; i < 4; i++) {
		_actualPositions[0] = _x + (i*_sprite.getWidth() * 2);
	}
}

void ScrollManager::update(double deltaTime)
{
	if (_active && _dir != 0) {
		for (int i = 0; i < 4; i++) {
			_actualPositions[i] += _dir * _vel*deltaTime;
			if (_dir > 0 && _actualPositions[i] >= _targetPositions[i] ||
				_dir < 0 && _actualPositions[i] <= _targetPositions[i] || _dir == 0) {
				_actualPositions[i] = _targetPositions[i];
				_dir = 0;
				// send msg dir + send msg puertas -> visible
			}

			setDirty();
		}
	}
}

void ScrollManager::render(RendererThread * renderThread)
{
	if (_active && _pendingFrames >= 0) {
		for (int i = 0; i < 4; i++) {
			_sprite.render(_actualPositions[i], _y, renderThread);
		}
		_pendingFrames--;
	}
}

void ScrollManager::handleInput()
{
	if (_active) {
		_inputData = Input::getUserInput();

		if (!_scrollingLeft && _inputData.buttonsInfo.R1) { // scroll de la camara hacia la derecha
			_scrollingRight = true;                         // (las puertas se moveran hacia la izquierda)
			_dir = -1;
			setNextTargetPositions();
		}
		else if (!_scrollingRight && _inputData.buttonsInfo.L1) { // scroll de la camara hacia la izquierda
			_scrollingLeft = true;                                // (las puertas se moveran hacia la derecha)
			_dir = 1;
			setNextTargetPositions();
		}

		else if (_scrollingRight && !_inputData.buttonsInfo.R1) {
			_scrollingRight = false;
		}
		else if (_scrollingLeft && !_inputData.buttonsInfo.L1) {
			_scrollingLeft = false;
		}
	}
}
