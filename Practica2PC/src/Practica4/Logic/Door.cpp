#include <Logic/Door.h>
#include <math.h>

bool const Door::isClosed() const
{
	return !_spriteSheet.isAnimated() && _spriteSheet.getCurrentFrame() == 0;
}

void Door::openDoor()
{
	if (isClosed()) {
		_spriteSheet.setAnim("opening");
		DoorOpeningMessage m(DOOR_OPENING, getId(), rand() % 2, getX(), getY()); //elige si aparece un bandido / cliente
		sendMessage(m);
		setDirty();
	}
}

void Door::closeDoor()
{
	if (!isClosed()) {
		_spriteSheet.setAnim("closing");
		_isClosing = true;
		setDirty();
	}
}

void Door::adjustSpriteSheet()
{
	SpriteSheet::Rect r(0, 0, _spriteSheet.getRect().right, _spriteSheet.getRect().bottom);

	r.left += 32 * _spriteSheet.getCurrentFrame();
	r.right = 116 - r.left;
	setX(initialPosX + r.left);
	_spriteSheet.setCurrentRect(r);
	sendMessage(Message(SET_DIRTY));
}

Door::Door(): _id(0)
{
}

Door::~Door()
{
}

void Door::init()
{
	_spriteSheet.init(Resources::puertas, 1, 4, 0);
	SpriteSheet::AnimInfo openingAnimInfo(0.07f, 0, 3, false);
	_spriteSheet.addAnim("opening", openingAnimInfo);
	SpriteSheet::AnimInfo closingAnimInfo(0.07f, 3, 0, false);
	_spriteSheet.addAnim("closing", closingAnimInfo);
}

void Door::update(double deltaTime)
{
	GameObject::update(deltaTime);

	if (_active) {
		if (!isClosed()) {
			timer += deltaTime;

			if (timer >= timeToClose) {
				closeDoor();
				timer = 0;
			}
		}
		else if (_isClosing) {
			IDMessage m(DOOR_CLOSED, _id);
			sendMessage(m);
			_isClosing = false;
		}
		if (_spriteSheet.isAnimated()) adjustSpriteSheet();
	}
}

void Door::receiveMessage(const Message & message)
{
	switch (message.type)
	{
	case START_EVENT: {
		if (_active) {
			const IDMessage* startEvent = static_cast<const IDMessage*>(&message);
			if (startEvent->id == _id) {
				openDoor();
			}
		}
		break;
	}
	case SHOOT: {
		const IDMessage* shootMessage = static_cast<const IDMessage*>(&message);
		if (!isClosed() && shootMessage->id == _id) {
			sendMessage(message);
		}
		break;
	}
	case ACTIVATE_DOORS: {
		setActive(true);
		break;
	}
	case DEACTIVATE_DOORS: {
		setActive(false);
		break;
	}
	case CHANGE_DOOR_TIME: {
		const ChangeDoorTimeMessage* doorTimeMessage = static_cast<const ChangeDoorTimeMessage*>(&message);
		timeToClose = doorTimeMessage->time;
		break;
	}
	default:
		break;
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

void Door::reset()
{
	GameObject::reset();
	_spriteSheet.setFrame(0);
	adjustSpriteSheet();
	_isClosing = false;
	timer = 0;
}
