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
	StickInfo leftStick;
	TriggersInfo triggersInfo;
	ButtonsInfo buttonsInfo;
};