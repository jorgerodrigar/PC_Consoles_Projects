#pragma once
#include <stdint.h>
#include <Utils/Resources.h>
#include <map>
#include <string>

class RendererThread;

class Sprite {
public:
	Sprite();
	~Sprite();

	struct AnimInfo {
		std::string name;
		float rate;
		char iniFrame, endFrame;
		bool isLooped;
		char count;
		AnimInfo():rate(0), iniFrame(0), endFrame(0), isLooped(false), count(0) {};
		AnimInfo(float rate, char iniFrame, char endFrame, bool isLooped):rate(rate), iniFrame(iniFrame), endFrame(endFrame), isLooped(isLooped), count(0) {};
	};

	struct Rect {
		int left, top;
		int right, bottom;
		Rect() :left(0), top(0), right(0), bottom(0) {};
		Rect(int left, int top, int right, int bottom) :left(left), top(top), right(right), bottom(bottom) {};
	};

	void setImage(Resources::ImageId id);

	void init(Resources::ImageId id, char rows, char cols, char frame = 0, bool visible = true);

	void draw(int x, int y, RendererThread* renderThread);
	void draw(int x, int y, int left, int top, int right, int bottom, RendererThread * renderThread);
	void draw(int x, int y, char rows, char cols, char frame, RendererThread * renderThread);

	bool update(double deltaTime);
	void render(int x, int y, RendererThread* renderThread);

	void addAnim(std::string name, AnimInfo& animInfo);
	void setAnim(std::string name);
	void setFrame(char frame);

	void setCurrentRect(Rect rect);
	Rect getRect();

	int getWidth();
	int getHeight();
	int const getFrameWidth() const;
	int const getFrameHeight() const;
	int const getCurrentFrame() const;
	std::string getCurrentAnimName() const;
	bool const getVisible() const;
	bool const isAnimated() const;


	void const setVisible(bool value);

	void sourceInWidthBounds(int& x, int boundMin, int boundMax);

private:
	uint32_t* _image;
	AnimInfo _currentAnim;
	Rect _srcRect;
	Rect _currentSrcRect;
	float _lastTime; 
	bool _isAnimated, _isVisible;
	int  _width, _height;
	char _rows, _cols,_currentFrame;

	std::map<std::string, AnimInfo> _animations;

	void changeFrame();
	void sendDrawCommand(int x, int y, int left, int top, int right, int bottom, RendererThread * renderThread);
};