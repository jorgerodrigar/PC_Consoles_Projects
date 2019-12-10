#include <Logic/GameObject.h>
#include <Logic/Sprite.h>
#include <Utils/Listener.h>

class DoorEvent : public GameObject, public Listener {
private:
	Sprite _door, _client, _bandit;
	unsigned int _centerX, _centerY;
	unsigned char _id;
	bool _isClosed = true;
	float timeToClose = 5.0f;
	float timer = 0;

	void throwRandomEvent(); //lanza un evento aleatorio (50% bandido, 50% cliente)
	void checkShot();
	void openDoor();
	void closeDoor();

	bool const isClient() const;

public:
	DoorEvent();
	~DoorEvent();
	virtual void init();
	virtual void render(RendererThread* renderThread);
	virtual void forceRender(RendererThread* renderThread);
	virtual void update(double deltaTime);

	virtual bool receiveMessage(const Message& message);

	void startRandomEvent();
	bool const isClosed() const;
	unsigned char const getId() const;
	void setId(unsigned char value);
};