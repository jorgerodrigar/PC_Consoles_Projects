#pragma once

struct StickInfo {
	float x, y;
};

struct TriggersInfo {
	float L2, R2;
};

struct ButtonsInfo {
	float R1, L1, SQUARE, CIRCLE, TRIANGLE, CROSS;
};

struct InputData
{
	InputData() {
		leftStick.x = leftStick.y = 0.0f;
		triggersInfo.L2 = triggersInfo.R2 = 0.0f;
		buttonsInfo.CIRCLE = buttonsInfo.CROSS = buttonsInfo.SQUARE = buttonsInfo.TRIANGLE = 0.0f;
		buttonsInfo.L1 = buttonsInfo.R1 = 0.0f;
	}

	StickInfo leftStick;
	TriggersInfo triggersInfo;
	ButtonsInfo buttonsInfo;
};