#include "DoorEvent.h"
#include <Utils/Resources.h>
#include <iostream>

void DoorEvent::throwRandomEvent()
{
	unsigned int rnd = rand() % 2;
	if (rand) {
		//_client.setVisible(true);
	}
	else {
		//_bandit.setVisible(true);
	}
}

bool const DoorEvent::isClient() const
{
	return _client.getVisible();
}

void DoorEvent::checkShot()
{
	if (isClient()) {
		//pierde
		std::cout << "Pierdesss" << std::endl;
	}
	else {
		//si te esta disparando... si no....
		std::cout << "GJ BRO" << std::endl;
	}
}

DoorEvent::DoorEvent(): GameObject()
{
}

DoorEvent::~DoorEvent()
{
}

void DoorEvent::init()
{
	_sprite.init(Resources::marcoPuerta, 1, 1);
	_door.init(Resources::puertas, 1, 4, 0);
	Sprite::AnimInfo openingAnimInfo(1, 0, 3, false);
	_door.addAnim("opening", openingAnimInfo);
	Sprite::AnimInfo closingAnimInfo(1, 3, 0, false);
	_door.addAnim("closing", closingAnimInfo);

	_client.init(Resources::cliente, 1, 3, 0, false);
	_bandit.init(Resources::ladron, 1, 5, 0, false);

	_centerX = _sprite.getWidth()/2 - (_door.getFrameWidth()/2); //provisional xd
	_centerY = _sprite.getHeight() / 2 - (_door.getFrameHeight()/5);
}

void DoorEvent::render(RendererThread * renderThread)
{
	if (_hasChanged && _active) {
		_sprite.render(_x, _y, renderThread);
		_door.render(_x + _centerX, _y + _centerY, renderThread);
		_client.render(_x + _centerX, _y + _centerY, renderThread);
		_bandit.render(_x + _centerX, _y + _centerY, renderThread);
		_hasChanged = false;
	}
}

void DoorEvent::forceRender(RendererThread * renderThread)
{
	if (_active) {
		_sprite.render(_x, _y, renderThread);
		_door.render(_x + _centerX, _y + _centerY, renderThread);
		_client.render(_x + _centerX, _y + _centerY, renderThread);
		_bandit.render(_x + _centerX, _y + _centerY, renderThread);
		_hasChanged = false;
	}
}

void DoorEvent::update(double deltaTime)
{
	if (!isClosed()) {
		timer += deltaTime;
		if (timer >= timeToClose) {
			//bandido dispara, cliente deposita dinero i guess
			closeDoor();
		}
	}
	bool aux = _sprite.update(deltaTime) || _door.update(deltaTime) || _client.update(deltaTime);
	if (!_hasChanged)
		_hasChanged = aux;
	//setX(getX() + (deltaTime * 100));
}

///Receptor de mensajes de la puerta.
///CASE MESSAGETYPE::SHOOT --> si el id corresponde con el actual de la puerta, procesa el mensaje y comprueba
///si ha disparado a un cliente o a un bandido. Si no corresponde, devuelve falso ya que no procesa el mensaje.
bool DoorEvent::receiveMessage(const Message & message)
{
	switch (message.type)
	{
	case MessageType::SHOOT: 
	{
		const ShootMessage* shootMessage = static_cast<const ShootMessage*>(&message);
		if (shootMessage->id == getId()) {
			checkShot();
			return true;
		}
		return false;
	}
	default:
		return false;
	}
}

void DoorEvent::openDoor()
{
	_door.setAnim("opening");
	_isClosed = false;
}

void DoorEvent::closeDoor()
{
	_door.setAnim("closing");
	_bandit.setVisible(false);
	_client.setVisible(false);
	_isClosed = true;
}

void DoorEvent::startRandomEvent()
{
	openDoor(); //starts opening animation 
	throwRandomEvent(); //throws a client / bandit event
}

bool const DoorEvent::isClosed() const
{
	return _isClosed;
}

unsigned char const DoorEvent::getId() const
{
	return _id;
}

void DoorEvent::setId(unsigned char value)
{
	_id = value;
}
