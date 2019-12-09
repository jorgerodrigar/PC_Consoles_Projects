#include <Logic/GameObject.h>
#include <Logic/Sprite.h>

class DoorEvent : public GameObject {
private:
	Sprite _door, _client;

public:
	DoorEvent();
	~DoorEvent();
	virtual void init();
	virtual void render(RendererThread* renderThread);
	virtual void forceRender(RendererThread* renderThread);
};