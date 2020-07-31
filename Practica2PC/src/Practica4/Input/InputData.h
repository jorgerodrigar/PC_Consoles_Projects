#pragma once

///informacion del estado de los sticks analogicos
struct StickInfo {
	float x, y;
};

///informacion del estado de los gatillos
struct TriggersInfo {
	float L2, R2;
};

///informacion del estado de los botones
struct ButtonsInfo {
	float R1, L1, SQUARE, CIRCLE, TRIANGLE, CROSS;
};

///informacion del input independiente de la plataforma. Cada input lo rellena a su manera 
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