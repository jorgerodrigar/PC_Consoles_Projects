#include "GameManager.h"
#include <Logic/Bandit.h>
#include <Logic/Bang.h>
#include <Logic/Client.h>
#include <Logic/DollarHUD.h>
#include <Logic/Door.h>
#include <Logic/GameObject.h>
#include <Utils/Message.h>
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

GameManager * GameManager::getInstance()
{
	if (_instance == nullptr)
		_instance = new GameManager();

	return _instance;
}

void GameManager::release()
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
	_round = 0;
	_iniTimePerEvent = 2;
	_iniTimeToClose = 3;
	_firstSeenDoor = 0;
	_numOfDollars = 9;
	_numOfVisibleDoors = 3;

	ScrollManager* scroll = new ScrollManager(100.0f);
	scroll->init();
	scroll->addListener(this);
	_gameObjects.push_back(scroll);

	Shooter* shooter = new Shooter();
	shooter->init();
	_gameObjects.push_back(shooter);

	/*Bang* bang = new Bang();
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
	_gameObjects.push_back(bandit);*/

	for (int i = 0; i < _numOfDollars; i++) {
		DollarHUD* dollarHud = new DollarHUD();
		dollarHud->init();
		dollarHud->setX(_minBound + (i * 64 * 2));
		dollarHud->setId(i);
		addListener(dollarHud);
		_gameObjects.push_back(dollarHud);
	}

	/*for (int i = 0; i < _numOfVisibleDoors; i++) {
		Door* door = new Door();
		door->init();
		door->setId(i);
		door->setY(48 * 2);
		door->setX(64 * 2 + (192 * i * 2));
		door->setInitialPosX(door->getX());
		addListener(door);
		door->addListener(client);
		door->addListener(bandit);
		door->addListener(this);
		scroll->addListener(door);
		shooter->addListener(door);
		_gameObjects.push_back(door);
	}*/

	reset();
}

void GameManager::reset()
{
	_timePerEvent = _iniTimePerEvent - (_round * _accFactor);
	_timeToClose = _iniTimeToClose - (_round * _accFactor);
	_round++;
	_currentTime = 0;
	_isEventActive = _gameOver = false;
	_gameOver = false;
	_firstSeenDoor = 0;

	for (int i = 0; i < _gameObjects.size(); i++) {
		_gameObjects[i]->reset();
	}

	setSeenDollars();
}

void GameManager::update(double deltaTime)
{
	if(!_isEventActive && !_gameOver) _currentTime += deltaTime;

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

bool GameManager::allDoorsClosed()
{
	return true;
}

void GameManager::getGameBounds(float & minBound, float & maxBound)
{
	minBound = _minBound;
	maxBound = _maxBound;
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
	case SCROLL_FINISHED:
	{
		const ScrollMessage* msg = static_cast<const ScrollMessage*>(&message);
		_firstSeenDoor += msg->dir;
		if (_firstSeenDoor < 0)_firstSeenDoor = _numOfDollars - 1;
		else if (_firstSeenDoor >= _numOfDollars)_firstSeenDoor = 0;
		setSeenDollars();

		break;
	}
	default:
		break;
	}
}

void GameManager::activateRandomEvent()
{
	unsigned char rnd = rand() % _numOfVisibleDoors;

	_isEventActive = true;
	StartEventMessage m(START_EVENT, rnd);
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
