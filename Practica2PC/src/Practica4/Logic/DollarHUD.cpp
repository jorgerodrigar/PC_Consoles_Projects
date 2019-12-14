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
	_sprite.init(Resources::dolares, 1, 6);
	Sprite::AnimInfo animInfo(0.2, 6, 3, false);
	_sprite.addAnim("money", animInfo);

	_id = 0;
	reset();
}

void DollarHUD::reset()
{
	GameObject::reset();
	setUnSelected();
	_sprite.setFrame(0);
}

void DollarHUD::setSelected()
{
	if (!_selected) {
		_selected = true;
		char currentFrame = _sprite.getCurrentFrame();
		_sprite.setFrame(currentFrame + 1);
		setDirty();
	}
}

void DollarHUD::setUnSelected()
{
	if (_selected) {
		_selected = false;
		char currentFrame = _sprite.getCurrentFrame();
		_sprite.setFrame(currentFrame - 1);
		setDirty();
	}
}

void DollarHUD::depositMoney()
{
	if (_selected)
		_sprite.setAnim("money");
}

void DollarHUD::setId(char id)
{
	_id = id;
}

char DollarHUD::getId()
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
		const DepositMessage* msg = static_cast<const DepositMessage*>(&message);
		if (msg->id == _id) {
			depositMoney();
		}
		break;
	}
	default:
		break;
	}
}
