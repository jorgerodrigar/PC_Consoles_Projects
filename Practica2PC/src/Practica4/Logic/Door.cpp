#include <Logic/Door.h>
#include <math.h>

bool const Door::isClosed() const
{
	return !_sprite.isAnimated() && _sprite.getCurrentFrame() == 0;
}

void Door::openDoor()
{
	if (isClosed()) {
		_sprite.setAnim("opening");
		DoorOpeningMessage m(DOOR_OPENING, getId(), rand() % 2); //elige si aparece un bandido / cliente
		sendMessage(m);
		setDirty();
	}
}

void Door::closeDoor()
{
	if (!isClosed()) {
		_sprite.setAnim("closing");
		_isClosing = true;
		setDirty();
	}
}

void Door::adjustSprite()
{
	Sprite::Rect r(0, 0, _sprite.getRect().right, _sprite.getRect().bottom);

	r.left += 32 * _sprite.getCurrentFrame();
	r.right = 116 - r.left;
	setX(initialPosX + r.left * 2);
	_sprite.setCurrentRect(r);
	sendMessage(SetDirtyMessage(SET_DIRTY));
}

Door::Door()
{
}

Door::~Door()
{
}

void Door::init()
{
	_sprite.init(Resources::puertas, 1, 4, 0);
	Sprite::AnimInfo openingAnimInfo(0.1f, 0, 3, false);
	_sprite.addAnim("opening", openingAnimInfo);
	Sprite::AnimInfo closingAnimInfo(0.1f, 3, 0, false);
	_sprite.addAnim("closing", closingAnimInfo);
}

void Door::update(double deltaTime)
{
	GameObject::update(deltaTime);

	if (!isClosed()) {
		timer += deltaTime;

		if (timer >= timeToClose) {
			closeDoor();
			timer = 0;
		}
	}
	else if(_isClosing){
		DoorClosedMessage m(DOOR_CLOSED, _id);
		sendMessage(m);
		_isClosing = false;
	}
	adjustSprite();
}

bool Door::receiveMessage(const Message & message)
{
	switch (message.type)
	{
	case START_EVENT: {
		const StartEventMessage* startEvent = static_cast<const StartEventMessage*>(&message);
		if (startEvent->id == _id) {
			openDoor();
			return true;
		}
		return false;
	}
	case SHOOT: {
		const ShootMessage* shootMessage = static_cast<const ShootMessage*>(&message);
		if (shootMessage->id == _id) {
			sendMessage(message);
			return true;
		}
		return false;
	}
	default:
		return false;
	}
}

unsigned char const Door::getId() const
{
	return _id;
}

void Door::setId(unsigned char value)
{
	_id = value;
}

void Door::setInitialPosX(float value)
{
	initialPosX = value;
}
