#include <Logic/GameObject.h>
#include <Logic/Sprite.h>
#include <Utils/Listener.h>

class DoorEvent : public GameObject, public Listener {
private:
	Sprite _door, _client;
	unsigned int _centerX, _centerY;
	bool _isClosed = true;

public:
	DoorEvent();
	~DoorEvent();
	virtual void init();
	virtual void render(RendererThread* renderThread);
	virtual void forceRender(RendererThread* renderThread);
	virtual void update(double deltaTime);

	virtual bool receiveMessage(const Message& message);

	void openDoor();
	void closeDoor();
	bool isClosed();
};