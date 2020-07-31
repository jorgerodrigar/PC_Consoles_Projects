#include "GameManager.h"
#include <Logic/Bandit.h>
#include <Logic/Bang.h>
#include <Logic/Client.h>
#include <Logic/DollarHUD.h>
#include <Logic/Door.h>
#include <Logic/GameObject.h>
#include <Logic/Message.h>
#include <Logic/ScrollManager.h>
#include <Logic/Shooter.h>
#include <Utils/Resources.h>
#include <Renderer/RendererThread.h>
#include <iostream>

GameManager* GameManager::_instance = nullptr;

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
	for (int i = 0; i < _gameObjects.size(); i++) {
		delete _gameObjects[i];
		_gameObjects[i] = nullptr;
	}
}

GameManager * const GameManager::GetInstance()
{
	if (_instance == nullptr)
		_instance = new GameManager();

	return _instance;
}

void GameManager::Release()
{
	if (_instance != nullptr) {
		delete _instance; _instance = nullptr;
	}
}

void GameManager::init(RendererThread* rendererThread)
{
	_renderThread = rendererThread;
	_gameObjects = std::vector<GameObject*>();

	_accFactor = 0.1;
	_iniTimePerEvent = 2;
	_iniTimeToClose = 2;
	_firstSeenDoor = 0;
	_numOfDollars = 9;
	_numOfVisibleDoors = 3;
	_round = -1;
	_gameOver = false;

	ScrollManager* scroll = new ScrollManager(100.0f);
	scroll->init();
	scroll->addListener(this);
	_gameObjects.push_back(scroll);

	Shooter* shooter = new Shooter();
	shooter->init();
	_gameObjects.push_back(shooter);

	Bang* bang = new Bang();
	bang->init();
	bang->addListener(this);
	_gameObjects.push_back(bang);

	Client* client = new Client();
	client->init();
	client->addListener(this);
	client->addListener(bang);
	_gameObjects.push_back(client);

	Bandit* bandit = new Bandit();
	bandit->init();
	bandit->addListener(this);
	bandit->addListener(bang);
	_gameObjects.push_back(bandit);

	for (int i = 0; i < _numOfDollars; i++) {
		DollarHUD* dollarHud = new DollarHUD();
		dollarHud->init();
		dollarHud->setX(MIN_BOUND + (i * 64));
		dollarHud->setId(i);
		addListener(dollarHud);
		dollarHud->addListener(this);
		_gameObjects.push_back(dollarHud);
	}

	for (int i = 0; i < _numOfVisibleDoors; i++) {
		Door* door = new Door();
		door->init();
		door->setId(i);
		door->setY(72);
		door->setX(64 + (192 * i));
		door->setInitialPosX(door->getX());
		addListener(door);
		door->addListener(client);
		door->addListener(bandit);
		door->addListener(this);
		scroll->addListener(door);
		shooter->addListener(door);
		_gameObjects.push_back(door);
	}

	reset();
}

void GameManager::reset()
{
	if (!_gameOver) {
		_round++;
	}
	else _round = 0;

	_timePerEvent = _iniTimePerEvent - (_round * _accFactor);
	_timeToClose = _iniTimeToClose - (_round * _accFactor);

	_currentTime = 0;
	_isEventActive = _gameOver = false;
	_gameOver = false;
	_firstSeenDoor = 0;
	_isScrolling = false;
	_dollarsInserted = 0;

	for (int i = 0; i < _gameObjects.size(); i++) {
		_gameObjects[i]->reset();
	}

	sendMessage(ChangeDoorTimeMessage(CHANGE_DOOR_TIME, _timeToClose));
	setSeenDollars();
}

void GameManager::update(double deltaTime)
{
	if(!_isEventActive && !_gameOver && !_isScrolling) _currentTime += deltaTime;

	if (_currentTime >= _timePerEvent) {
		activateRandomEvent();
		_currentTime = 0;
	}

	for (int i = 0; i < _gameObjects.size(); i++) {
		_gameObjects[i]->update(deltaTime);
	}
}

void GameManager::render()
{
	for (int i = 0; i < _gameObjects.size(); i++) {
		_gameObjects[i]->render(_renderThread);
	}
}

void GameManager::handleInput()
{
	for (int i = 0; i < _gameObjects.size(); i++) {
		_gameObjects[i]->handleInput();
	}
}

bool const GameManager::allDoorsClosed() const
{
	return !_isEventActive;
}

void GameManager::receiveMessage(const Message & message)
{
	switch (message.type)
	{
	case START_ROUND:
	{
		reset();
		break;
	}
	case GAME_OVER:
	{
		gameOver();
		break;
	}
	case DOOR_CLOSED:
	{
		_isEventActive = false;
		break;
	}
	case SCROLL_STARTED:
	{
		const ScrollMessage* msg = static_cast<const ScrollMessage*>(&message);
		_isScrolling = true;
		_firstSeenDoor -= msg->dir;
		_currentTime = 0;

		if (_firstSeenDoor < 0)_firstSeenDoor = _numOfDollars - 1;
		else if (_firstSeenDoor >= _numOfDollars)_firstSeenDoor = 0;
		setSeenDollars();
		break;
	}
	case SCROLL_FINISHED: {
		_isScrolling = false;
		break;
	}
	case DEPOSIT: {
		const IDMessage* msg = static_cast<const IDMessage*>(&message);
		char dollarId = msg->id + _firstSeenDoor;
		if (dollarId >= _numOfDollars) dollarId = dollarId - _numOfDollars;
		sendMessage(IDMessage(DEPOSIT, dollarId));
		break;
	}
	case MONEY_INSERTED: {
		_dollarsInserted++;
		if (_dollarsInserted == _numOfDollars) reset();
	}
	default:
		break;
	}
}

void GameManager::activateRandomEvent()
{
	unsigned char rnd = rand() % _numOfVisibleDoors;

	_isEventActive = true;
	IDMessage m(START_EVENT, rnd);
	sendMessage(m);
}

void GameManager::setSeenDollars()
{
	char ids[3];
	for (int i = 0; i < _numOfVisibleDoors; i++) {
		ids[i] = _firstSeenDoor + i;
		if (ids[i] >= _numOfDollars)ids[i] = _firstSeenDoor - _numOfDollars + i;
	}
	SelectDollarsMessage m(SELECT_DOLLARS, ids);
	sendMessage(m);
}

void GameManager::gameOver()
{
	RendererThread::RenderCommand command;
	command.type = RendererThread::CLEAR;
	command.params.color = 0x00000000;
	_renderThread->enqueueCommand(command);

	for (int i = 0; i < _gameObjects.size(); i++) {
		_gameObjects[i]->setActive(false);
	}

	_gameOver = true;
	_currentTime = 0;
}
