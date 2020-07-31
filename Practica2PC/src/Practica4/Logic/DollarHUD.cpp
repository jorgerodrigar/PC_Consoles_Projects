#include "DollarHUD.h"
#include <Renderer/Renderer.h>

DollarHUD::DollarHUD()
{
}

DollarHUD::~DollarHUD()
{
}

void DollarHUD::init()
{
	_spriteSheet.init(Resources::dolares, 1, 6);
	SpriteSheet::AnimInfo animInfo(0.2, 5, 3, false);
	_spriteSheet.addAnim("money", animInfo);
	_id = 0;
}

void DollarHUD::update(double deltaTime)
{
	GameObject::update(deltaTime);

	if (_lateUnselect)
		setUnSelected();

	if (_moneyInserted && !_messageSent) {
		sendMessage(Message(MONEY_INSERTED));
		_spriteSheet.stopAnimation();
		_moneyInserted = false;
		_messageSent = true;
	}

	if (_spriteSheet.isAnimated() && _spriteSheet.getCurrentFrame() == 3) {
		_moneyInserted = true;
	}
}

void DollarHUD::reset()
{
	GameObject::reset();
	_selected = false;
	_moneyInserted = false;
	_lateUnselect = false;
	_messageSent = false;
	_spriteSheet.setFrame(0);
}

void DollarHUD::setSelected()
{
	if (!_selected) {
		_selected = true;
		char currentFrame = _spriteSheet.getCurrentFrame();
		_spriteSheet.setFrame(currentFrame + 1);
		setDirty();
	}
}

void DollarHUD::setUnSelected()
{
	if (_selected && !_spriteSheet.isAnimated()) {
		_selected = false;
		_lateUnselect = false;
		char currentFrame = _spriteSheet.getCurrentFrame();
		_spriteSheet.setFrame(currentFrame - 1);
		setDirty();
	}
	else if (_spriteSheet.isAnimated())_lateUnselect = true;
}

void DollarHUD::depositMoney()
{
	if (_selected) {
		_spriteSheet.setAnim("money");
		setDirty();
	}
}

void DollarHUD::setId(char id)
{
	_id = id;
}

char const DollarHUD::getId() const
{
	return _id;
}

void DollarHUD::receiveMessage(const Message & message)
{
	switch (message.type) {
	case SELECT_DOLLARS: {
		const SelectDollarsMessage* msg = static_cast<const SelectDollarsMessage*>(&message);
		const char* ids = msg->ids;
		int i = 0;
		while (_id != ids[i] && i < 3)i++;
		if (i < 3)setSelected();
		else setUnSelected();
		break;
	}
	case DEPOSIT: {
		const IDMessage* msg = static_cast<const IDMessage*>(&message);
		if (msg->id == _id) {
			depositMoney();
		}
		break;
	}
	default:
		break;
	}
}
