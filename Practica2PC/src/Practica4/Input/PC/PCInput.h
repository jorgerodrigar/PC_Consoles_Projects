#pragma once
#include <Input/InputData.h>
#include <Input/PC/InputListener.h>
class PCInput
{
public:
	static void init();
	static void release();
	static void tick();
	static InputData getUserInput();
	static InputListener _inputListener;

private:
	PCInput();
	~PCInput();
};

