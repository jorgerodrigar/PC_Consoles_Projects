#ifndef PCRENDERER_H_  
#define PCRENDERER_H_

class PCRenderer {

public:
	PCRenderer();
	~PCRenderer();
	void init();
	void release();

	void clear(int color);
	void putPixel(int x, int y, int color);
	void present();

	int getWindowWidth();
	int getWindowHeight();
};

#endif 