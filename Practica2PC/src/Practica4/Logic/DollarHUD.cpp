#include "DollarHUD.h"

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

	_selected = false;
}

void DollarHUD::switchSelected()
{
	_selected = !_selected;
	char currentFrame = _sprite.getCurrentFrame();

	if (_selected)
		_sprite.setFrame(currentFrame + 1);
	else
		_sprite.setFrame(currentFrame - 1);

	_hasChanged = true;
}

void DollarHUD::depositMoney()
{
	if (_selected)
		_sprite.setAnim("money");
}
