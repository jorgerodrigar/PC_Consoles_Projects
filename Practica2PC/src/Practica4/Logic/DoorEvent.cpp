#include "DoorEvent.h"
#include <Utils/Resources.h>
#include <iostream>
#include <Renderer/Renderer.h>

//Resetea el sprite actual del evento elegido anteriormente.
//Si es un cliente, lo hace visible y setea su animacion a "idle". Si es un bandido, 
//hace un random y, dependiendo del mismo, decide si aparece apuntando al jugador o no.
void DoorEvent::resetCurrentEventSprite()
{
	_currentEventSprite->setVisible(true);
	_currentEventSprite->setAnim("idle");

	if (_currentEventSprite == &_bandit) {
		unsigned char rnd = rand() % 100;
		if (rnd < 40) {
			_currentEventSprite->setAnim("aiming");
		}
	}
}

void DoorEvent::throwRandomEvent()
{
	unsigned char rnd = rand() % 2;
	if (rnd) {
		_currentEventSprite = &_client;
	}
	else {
		_currentEventSprite = &_bandit;
	}
	resetCurrentEventSprite();
}

void DoorEvent::adjustCurrentEventSprite()
{
	Sprite::Rect r(0, 0, _currentEventSprite->getRect().right/3, _currentEventSprite->getRect().bottom);

	switch (_door.getCurrentFrame())
	{
	case 0:
		r.right = _client.getRect().left;
		_currentEventSprite->setCurrentRect(r);
		break;
	case 1:
		_currentEventSprite->setCurrentRect(r);
		break;
	case 2:
		r.right = _currentEventSprite->getRect().right * (2.0f / 3.0f);
		_currentEventSprite->setCurrentRect(r);
		break;
	case 3:
		r.right = _currentEventSprite->getRect().right;
		_currentEventSprite->setCurrentRect(r);
		break;
	default:
		break;
	}
}

bool const DoorEvent::isValidTarget() const
{
	return (_currentEventSprite != &_client && _currentEventSprite->getCurrentFrame() == 1); 
	//o currentSprite == &_client. anyadir tambien si el bandido te esta apuntando o no maybe (TODO: USAR MEJOR EL NOMBRE DE LA ANIMACION?)
}

void DoorEvent::checkShot()
{
	if (!isValidTarget()) {
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
	Sprite::AnimInfo openingAnimInfo(0.1f, 0, 3, false);
	_door.addAnim("opening", openingAnimInfo);
	Sprite::AnimInfo closingAnimInfo(0.1f, 3, 0, false);
	_door.addAnim("closing", closingAnimInfo);
	//_door.setAnim("opening");
	_client.init(Resources::cliente, 1, 3, 0, false);
	Sprite::AnimInfo idleAnimInfo(0, 0, 0, false);
	_client.addAnim("idle", idleAnimInfo);
	Sprite::AnimInfo dyingAnimInfo(0.8f, 1, 2, false);
	_client.addAnim("dying", dyingAnimInfo);

	_bandit.init(Resources::ladron, 1, 5, 0, false);
	_bandit.addAnim("idle", idleAnimInfo);
	Sprite::AnimInfo aimingAnimInfo(0, 1, 1, false);
	_bandit.addAnim("aiming", aimingAnimInfo);
	dyingAnimInfo.iniFrame = 2; 
	dyingAnimInfo.endFrame = 3;
	_bandit.addAnim("dying", dyingAnimInfo);

	_centerX = getX() + (32 * 2); //provisional xd
	_centerY = getY() + (24 * 2);

	_currentEventSprite = &_client;
}

void DoorEvent::render(RendererThread * renderThread)
{
	if (_pendingFrames > 0 && _active) {
		_sprite.render(_x, _y, renderThread);
		_door.render(_x + _centerX, _y + _centerY, renderThread);
		_currentEventSprite->render(_x + _centerX, _y + _centerY, renderThread);
		//std::cout << "  " << _door.getCurrentFrame() << std::endl;

		_pendingFrames--;
	}
}

void DoorEvent::update(double deltaTime)
{
	//std::cout << deltaTime << std::endl;
	if (!isClosed()) {
		timer += deltaTime;

		if (timer >= timeToClose) {
			//bandido dispara, cliente deposita dinero i guess
			closeDoor();
			timer = 0;
		}
	}

	bool aux = _sprite.update(deltaTime) || _door.update(deltaTime) || _currentEventSprite->update(deltaTime);

	if (aux)
		_pendingFrames = Renderer::getNumBuffers();

	adjustCurrentEventSprite();
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
			_currentEventSprite->setAnim("dying");
			_pendingFrames = Renderer::getNumBuffers();
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
	_isClosed = true;
}

void DoorEvent::startRandomEvent()
{
	openDoor(); //starts opening animation 
	throwRandomEvent(); //throws a client / bandit event
}

bool const DoorEvent::isClosed() const
{
	return (!_door.isAnimated()) && (_door.getCurrentFrame() == 0);
}

unsigned char const DoorEvent::getId() const
{
	return _id;
}

void DoorEvent::setId(unsigned char value)
{
	_id = value;
}
