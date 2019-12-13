#include "GameObject.h"
#include <Utils/Listener.h>
#include <Utils/Emitter.h>

class Door : public GameObject, public Emitter, public Listener
{
private:
	unsigned char _id;
	float timeToClose = 5.0f;
	float timer = 0;
	float initialPosX;
	bool _isClosing = false;

	bool const isClosed() const;
	void openDoor();
	void closeDoor();
	void adjustSprite();

public:
	Door();
	~Door();

	virtual void init();
	virtual void update(double deltaTime);
	virtual bool receiveMessage(const Message& message);

	unsigned char const getId() const;
	void setId(unsigned char value);
	void setInitialPosX(float value);

};