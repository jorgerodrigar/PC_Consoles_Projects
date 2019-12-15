#pragma once
#include <Input/PC/InputListener.h>

class InputData;
class InputListener;

class PCInput
{
public:
	static void Init();
	static void Release();
	/// se pide la lista de eventos al listener de Platform y se rellena con ellos InputData
	static void Tick();
	static InputData* GetUserInput();

private:
	PCInput();
	~PCInput();

	// input en teclado
	static void HandleKeyDown(const SDL_Event& event);
	static void HandleKeyUp(const SDL_Event& event);

	// input en mando
	///maneja el evento de conexion del mando
	static void OpenController(const SDL_Event& event);
	static void HandleAxisMotion(const SDL_Event& event);
	static void HandleButtonDown(const SDL_Event& event);
	static void HandleButtonUp(const SDL_Event& event);

	static InputData* _inputData;
	static InputListener* _inputListener; // instancia de la clase listener de Platform
};

