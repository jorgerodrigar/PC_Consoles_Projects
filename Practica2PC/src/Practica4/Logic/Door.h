#include "GameObject.h"
#include <Logic/Listener.h>
#include <Logic/Emitter.h>

class Door : public GameObject, public Emitter, public Listener
{
private:
	unsigned char _id;
	float timeToClose = 5.0f;
	float timer = 0;
	float initialPosX;
	bool _isClosing = false;

	bool const isClosed() const;
	///abre la puerta y elige si va a aparecer un bandido o un cliente
	void openDoor();
	void closeDoor();
	///ajusta el rectangulo source dependiendo del frame en el que se encuentra (necesario para que se muestre el cliente / bandido y mantener coherencia de buffers)
	void adjustSpriteSheet();

public:
	Door();
	~Door();

	virtual void init();
	virtual void reset();

	virtual void update(double deltaTime);
	virtual void receiveMessage(const Message& message);

	unsigned char const getId() const;
	void setId(unsigned char value);
	void setInitialPosX(float value);
};