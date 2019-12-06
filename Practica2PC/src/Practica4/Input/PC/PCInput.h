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

	static void handleKeyDown(const SDL_Event& event);
	static void handleKeyUp(const SDL_Event& event);

	static void openController(const SDL_Event& event);
	static void handleAxisMotion(const SDL_Event& event);
	static void handleButtonDown(const SDL_Event& event);
	static void handleButtonUp(const SDL_Event& event);

	static InputData _inputData;
};

