#include "ScrollManager.h"
#include <Input/Input.h>
#include <Renderer/Renderer.h>
#include <Logic/GameManager.h>
#include <Input/InputData.h>

void ScrollManager::setNextTargetPositions()
{
	for (int i = 0; i < _numDoors; i++) {
		if (isOutsideBounds(_actualPositions[i])) {
			if (_dir > 0)
				_actualPositions[i] = (_minBound - (_spriteSheet.getWidth()));
			else if (_dir < 0)
				_actualPositions[i] = _maxBound;
		}
		_targetPositions[i] = _actualPositions[i] + (_dir * _spriteSheet.getWidth());
	}

	Message m(DEACTIVATE_DOORS);
	sendMessage(m);
}

bool ScrollManager::isOutsideBounds(float x)
{
	return (x >= _maxBound || x + (_spriteSheet.getWidth()) <= _minBound);
}

ScrollManager::ScrollManager() :_scrollingRight(false), _scrollingLeft(false),
_vel(0), _dir(0), _numDoors(0), _minBound(0), _maxBound(0)
{
}

ScrollManager::ScrollManager(float vel, int numDoors) : _scrollingRight(false), _scrollingLeft(false),
_vel(vel), _dir(0), _numDoors(numDoors), _minBound(0), _maxBound(Renderer::GetWindowWidth())
{
}

ScrollManager::~ScrollManager()
{
	delete _actualPositions; _actualPositions = nullptr;
	delete _targetPositions; _targetPositions = nullptr;
}

void ScrollManager::init()
{
	_gm = GameManager::GetInstance();
	_spriteSheet.init(Resources::marcoPuerta, 1, 1);

	_originalRect = SpriteSheet::Rect(0, 0, _spriteSheet.getWidth(), _spriteSheet.getHeight());

	_gm->getGameBounds(_minBound, _maxBound);
	_actualPositions = new float[_numDoors];
	_targetPositions = new float[_numDoors];

	_x = _minBound;
	_y = 48;
}

void ScrollManager::reset()
{
	GameObject::reset();

	for (int i = 0; i < _numDoors; i++) {
		_actualPositions[i] = _x + (i*_spriteSheet.getWidth());
	}
	_dir = 0;
}

void ScrollManager::update(double deltaTime)
{
	if (_active && _dir != 0) {
		for (int i = 0; i < _numDoors; i++) {
			_actualPositions[i] += _dir * _vel*deltaTime;
			if (_dir > 0 && _actualPositions[i] >= _targetPositions[i] ||
				_dir < 0 && _actualPositions[i] <= _targetPositions[i] || _dir == 0) {
				_actualPositions[i] = _targetPositions[i];
				if (_dir != 0) {
					if (!_scrollingRight && !_scrollingLeft) {
						Message m(ACTIVATE_DOORS);
						sendMessage(m);
					}
					Message scrollMsg(SCROLL_FINISHED);
					sendMessage(scrollMsg);
				}
				_dir = 0;
			}
		}
		setDirty();
	}
}

void ScrollManager::render(RendererThread * renderThread)
{
	if (_active && _pendingFrames >= 0) {
		for (int i = 0; i < _numDoors; i++) {
			float _x = _actualPositions[i];
			_spriteSheet.sourceInWidthBounds(_x, _minBound, _maxBound);
			_spriteSheet.render(_x, _y, renderThread);
			_spriteSheet.setCurrentRect(_originalRect);
		}
		_pendingFrames--;
	}
}

void ScrollManager::handleInput()
{
	if (_active) {
		_inputData = &Input::GetUserInput();

		if (!_scrollingLeft && _inputData->buttonsInfo.R1) { // scroll de la camara hacia la derecha
			_scrollingRight = true;                         // (las puertas se moveran hacia la izquierda)
			if (_dir == 0 && _gm->allDoorsClosed()) {
				_dir = -1;
				sendMessage(ScrollMessage(SCROLL_STARTED, _dir));
				setNextTargetPositions();
			}
		}
		else if (!_scrollingRight && _inputData->buttonsInfo.L1) { // scroll de la camara hacia la izquierda
			_scrollingLeft = true;                                // (las puertas se moveran hacia la derecha)
			if (_dir == 0 && _gm->allDoorsClosed()) {
				_dir = 1;
				sendMessage(ScrollMessage(SCROLL_STARTED, _dir));
				setNextTargetPositions();
			}
		}
		else if (_scrollingRight && !_inputData->buttonsInfo.R1) {
			_scrollingRight = false;
		}
		else if (_scrollingLeft && !_inputData->buttonsInfo.L1) {
			_scrollingLeft = false;
		}
	}
}
