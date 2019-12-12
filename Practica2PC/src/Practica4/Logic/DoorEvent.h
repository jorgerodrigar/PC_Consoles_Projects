#include <Logic/GameObject.h>
#include <Logic/Sprite.h>
#include <Utils/Listener.h>
#include <Utils/Emitter.h>

class DoorEvent : public GameObject, public Listener, public Emitter{
private:
	Sprite _door, _client, _bandit;
	Sprite* _currentEventSprite;
	unsigned int _centerX, _centerY;
	unsigned char _id;
	float timeToClose = 5.0f;
	float timer = 0;

	void resetCurrentEventSprite();
	void throwRandomEvent(); //lanza un evento aleatorio (50% bandido, 50% cliente)
	void adjustCurrentEventSprite();
	void processShot();
	void openDoor();
	void closeDoor();


public:
	DoorEvent();
	~DoorEvent();
	virtual void init();
	virtual void render(RendererThread* renderThread);
	virtual void update(double deltaTime);

	virtual bool receiveMessage(const Message& message);
	void startRandomEvent(); //opens the door and starts a random event

	bool const isClosed() const;
	bool const isValidTarget() const;
	unsigned char const getId() const;

	void setId(unsigned char value);
};