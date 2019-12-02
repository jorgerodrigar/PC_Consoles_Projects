#pragma once

#include <stdint.h>
#include <map>
#include <vector>

using namespace std;

class Resources
{
public:
	static Resources* getInstance();
	static void release();

	enum ImageId {
		debug2x2,
		debug500x500,
		bang, 
		cliente,
		dolares, 
		ladron,
		marcoPuerta,
		puertas
	};

	pair<uint32_t*, uint32_t*> getImage(ImageId id);

private:
	static Resources* _instance;
	map<ImageId, pair<uint32_t*, uint32_t*>> _images;

	static std::vector<std::string> _imageFiles; // initialized in .cpp

	Resources();
	~Resources();

	void load();
};

