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

	//TODO: SCROLL
	//ScrollManager* scroll = new ScrollManager();
	//scroll->init();
	//scroll->addListener(this);
	//_gameObjects.push_back(scroll);

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

	for (int i = 0; i < 9; i++) {
		DollarHUD* dollarHud = new DollarHUD();
		dollarHud->init();
		dollarHud->setX(32.0f * 2 + (i * 64 * 2));
		addListener(dollarHud);
		_gameObjects.push_back(dollarHud);
	}

	for (int i = 0; i < 3; i++) {
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
		//scroll.addListener(door);
		shooter->addListener(door);
		_gameObjects.push_back(door);
	}

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

	for (int i = 0; i < _gameObjects.size(); i++) {
		_gameObjects[i]->reset();
	}
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
	case LATERAL_SCROLL:
	{

		break;
	}
	default:
		break;
	}
}

void GameManager::activateRandomEvent()
{
	unsigned char rnd = rand() % 3;

	_isEventActive = true;
	StartEventMessage m(START_EVENT, rnd);
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
