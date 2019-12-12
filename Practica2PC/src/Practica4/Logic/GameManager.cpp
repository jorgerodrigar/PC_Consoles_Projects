#include "GameManager.h"
#include <Logic/DollarHUD.h>
#include <Logic/DoorEvent.h>
#include <Renderer/RendererThread.h>
#include <Utils/Message.h>
#include <Logic/Shooter.h>
#include <Logic/ScrollManager.h>
#include <Utils/Resources.h>

/*PROVISIONAL*/
#include <Input/Input.h>
#include <Input/InputData.h>
#include <Platform/Platform.h>
#include <Renderer/Renderer.h>
#include <iostream>
/*PROVISIONAL*/

GameManager* GameManager::_instance = nullptr;

GameManager::GameManager()
{
}


GameManager::~GameManager()
{
	delete _shooter; _shooter = nullptr;
	delete _scrollManager; _scrollManager = nullptr;
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
	_dollars = std::vector<DollarHUD>();
	_doors = std::vector<DoorEvent*>();
	_screenDoors = std::vector<ScreenDoor>();
	_shooter = new Shooter();

	_scrollManager = new ScrollManager(150.0f); //vel de scroll
	scrollDir = scrollVel = 0;

	_shooter->addListener(this);
	_scrollManager->addListener(this);

	_bangSprite.init(Resources::bang, 1, 6, 0, false);
	Sprite::AnimInfo bangAnimInfo(0.3f, 0, 5, false);
	_bangSprite.addAnim("bang", bangAnimInfo);

	_accFactor = 0.1;
	_round = 0;

	for (int i = 0; i < 9; i++) {
		addDollar(i);
	}

	for (int i = 0; i < 4; i++) {
		addDoor(i);
	}

	reset();
}

void GameManager::reset()
{
	_round++;
	_timePerEvent = 2 - (_round * _accFactor);
	_currentTime = 0;
	_isEventActive = _gameOver = false;
	_bangSprite.setVisible(false);
	_gameOver = false;
}

void GameManager::update(double deltaTime)
{
	if(!_isEventActive || _gameOver) _currentTime += deltaTime;

	if (_currentTime >= _timePerEvent) {
		if (!_gameOver) {
			activateRandomEvent();
			_currentTime = 0;
		}
		else gameOver(); 
	}

	for (int i = 0; i < _dollars.size(); i++) {
		_dollars[i].update(deltaTime);
	}
	
	_scrollManager->update();

	int i = 0;
	for (auto it = _doors.begin(); it != _doors.end(); it++) {
		(*it)->update(deltaTime);

		if ((*it)->getVel() != 0) {
			if (scrollDir > 0 && (*it)->getX() >= targetPositions[i] ||
				scrollDir < 0 && (*it)->getX() <= targetPositions[i]) {
				scrollVel = 0.0f;
				(*it)->setVel(scrollVel);
				(*it)->setX(targetPositions[i]);
			}
		}

		i++;
	}

	_bangSprite.update(deltaTime);
}

void GameManager::render()
{
	for (int i = 0; i < _dollars.size(); i++) {
		_dollars[i].render(_renderThread);
	}
	for (auto it = _doors.begin(); it!=_doors.end(); it++) {
		(*it)->render(_renderThread);
	}

	_bangSprite.render(640, 360, _renderThread); //TODO: recibir medidas
}

void GameManager::handleInput()
{
	_shooter->handleInput();
}

bool GameManager::receiveMessage(const Message & message)
{
	switch (message.type)
	{
	case SHOOT:
	{
		const ShootMessage* shootMessage = static_cast<const ShootMessage*>(&message);
		if (!_doors[shootMessage->id + 1]->isClosed()) {
			ShootMessage provMessage(SHOOT, shootMessage->id + 1);
			sendMessage(provMessage);
			//gameover si disparas a un target no valido (isvalidtarget)
		}
		return true;
	}
	case DOOR_CLOSING:
	{
		_isEventActive = false;
		
		const DoorClosingMessage* doorClosingMessage = static_cast<const DoorClosingMessage*>(&message);
		if (doorClosingMessage->id == -1) {
			/*_gameOver = true;
			_currentTime = 0;
			_timePerEvent = 2;*/
		}
		//si es 1, depositan dinero
		return true;
	}
	case MessageType::LATERAL_SCROLL:
	{
		const ScrollMessage* scrollMessage = static_cast<const ScrollMessage*>(&message);
		if (scrollVel == 0) {
			scrollDir = scrollMessage->dir;
			scrollVel = scrollMessage->vel;
			int i = 0;
			for (auto it = _doors.begin(); it != _doors.end(); it++) {
				if (isOutsideBounds((*it)->getX())) {
					if (scrollDir > 0)
						(*it)->setX((32.0f*2.0f) - (192.0f*2.0f)); // PROVISIONAL
					else if (scrollDir < 0)
						(*it)->setX(Renderer::getWindowWidth() - (32.0f*2.0f)); // PROVISIONAL
				}
				targetPositions[i] = (*it)->getX() + (scrollDir * 192 * 2);// PROVISIONAL
				(*it)->setVel(scrollDir*scrollVel);
				i++;
			}
		}
		return true;
	}
	default:
		return false;
	}
}

void GameManager::addDoor(int id)
{
	DoorEvent* go = new DoorEvent();
	go->init();
	go->setId(id);
	go->setY(48 * 2);
	go->setX(32 * 2 + (192 * id * 2));
	_doors.push_back(go);
	addListener(_doors[id]);
	_doors[id]->addListener(this);

	ScreenDoor screenDoor;
	screenDoor.currentId = id;
	screenDoor.currentDollar = id;
	_screenDoors.push_back(screenDoor);
}

void GameManager::addDollar(int id)
{
	DollarHUD go;
	go.init();
	go.setX(32.0f * 2 + (id * 64 * 2));
	_dollars.push_back(go);
}

void GameManager::activateRandomEvent()
{
	unsigned char rnd = rand() % 3;
	while (!_doors[_screenDoors[rnd].currentId]->isClosed()) rnd = rand() % 3;

	_isEventActive = true;
	_doors[_screenDoors[rnd].currentId]->startRandomEvent();
}

void GameManager::gameOver()
{
	if (!_bangSprite.isAnimated()) {
		_bangSprite.setVisible(true);
		_bangSprite.setAnim("bang");
	}
	std::cout << _bangSprite.getCurrentFrame() << std::endl;
	if (_bangSprite.getCurrentFrame() == 5)	reset();

	Renderer::clear(0x000000);
}

bool GameManager::isOutsideBounds(float x) // PROVISIONAL
{
	return (x >= Renderer::getWindowWidth() - 32.0f*2.0f || x + (192 * 2) <= 32.0f*2.0f);
}
