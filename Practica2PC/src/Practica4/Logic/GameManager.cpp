#include "GameManager.h"
#include <Logic/DollarHUD.h>
#include <Logic/DoorEvent.h>
#include <Renderer/RendererThread.h>
#include <Utils/Message.h>
#include <Logic/Shooter.h>

/*PROVISIONAL*/
#include <Input/Input.h>
#include <Input/InputData.h>
#include <Platform/Platform.h>
/*PROVISIONAL*/

GameManager* GameManager::_instance = nullptr;

GameManager::GameManager()
{
}


GameManager::~GameManager()
{
	delete _shooter; _shooter = nullptr;
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
	_shooter = new Shooter();

	_shooter->addListener(this);

	for (int i = 0; i < 9; i++) {
		DollarHUD go;
		go.init();
		go.setX(32.0f * 2 + (i * 64 * 2));
		_dollars.push_back(go);
	}

	for (int i = 0; i < 3; i++) {
		DoorEvent* go = new DoorEvent();
		go->init();
		go->setId(i);
		go->setY(48 * 2);
		go->setX(32 * 2 + (192 * i * 2));
		_doors.push_back(go);
		addListener(_doors[i]);
	}
}

void GameManager::update(double deltaTime)
{
	/*PROVISIONAL*/
	InputData data = Input::getUserInput();
	if (data.buttonsInfo.L1 == 1) {
		//kk = false;
		if (_doors[0]->isClosed())
			_doors[0]->startRandomEvent();
	}
	/*else kk = true;
	if (kk)
		frame--;
	else
		frame++;
	//go.setX(frame * 10);
	//std::cout << data.buttonsInfo.L1 << std::endl;
	if (!kk)
		_doors[1].setX(_doors[1].getX() + Platform::getDeltaTime() * 50);
	else
		_doors[1].setX(_doors[1].getX() - Platform::getDeltaTime() * 50);
	/*PROVISIONAL*/

	for (int i = 0; i < _dollars.size(); i++) {
		_dollars[i].update(deltaTime);
	}
	for (auto it = _doors.begin(); it != _doors.end(); it++) {
		(*it)->update(deltaTime);
	}
}

void GameManager::render()
{
	for (int i = 0; i < _dollars.size(); i++) {
		_dollars[i].render(_renderThread);
	}
	for (auto it = _doors.begin(); it!=_doors.end(); it++) {
		(*it)->render(_renderThread);
	}
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
		const DoorClosingMessage* doorClosingMessage = static_cast<const DoorClosingMessage*>(&message);
		if (!_doors[doorClosingMessage->id + 1]->isClosed()) {
			//gameover si es -1
			//si no depositan dinero
		}
		return true;
	}
	default:
		return false;
	}
}
