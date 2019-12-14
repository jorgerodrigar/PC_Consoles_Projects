#include "Bang.h"
#include <Utils/Resources.h>
#include <Utils/Message.h>

Bang::Bang()
{
}

Bang::~Bang()
{
}

void Bang::init()
{
	setX(256);
	setY(156);

	_sprite.init(Resources::bang, 1, 6, 0);
	Sprite::AnimInfo bangAnimInfo(0.35f, 0, 5, false);
	_sprite.addAnim("bang", bangAnimInfo);
}

void Bang::update(double deltaTime)
{
	GameObject::update(deltaTime);

	if (_active) {
		if (_sprite.isAnimated() && _sprite.getCurrentFrame() == 5) {
			Message m(START_ROUND);
			sendMessage(m);
		}
	}
}

void Bang::reset()
{
	setActive(false);
}

void Bang::receiveMessage(const Message & message)
{
	switch (message.type)
	{
	case GAME_OVER:
		setActive(true);
		setDirty();
		_sprite.setAnim("bang");
		break;
	default:
		break;
	}
}
