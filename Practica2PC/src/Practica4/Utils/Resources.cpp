#include "Resources.h"
#include <Platform/Platform.h>
#include <Utils/BinReader.h>
#include <iostream>


Resources* Resources::_instance = nullptr;

std::vector<std::string> Resources::_imageFiles{ "assets/SpritesSheetsWestBank/Debug/rgb.rgba",
"assets/SpritesSheetsWestBank/Debug/rgb_500x500.rgba",
"assets/SpritesSheetsWestBank/bang.rgba",
"assets/SpritesSheetsWestBank/client.rgba",
"assets/SpritesSheetsWestBank/dolares.rgba",
"assets/SpritesSheetsWestBank/ladron.rgba",
"assets/SpritesSheetsWestBank/marcoPuerta.rgba",
"assets/SpritesSheetsWestBank/puertas.rgba" };

Resources::Resources()
{
	load();
}

Resources::~Resources()
{
	for (map<ImageId, pair<uint32_t*, uint32_t*>>::iterator it = _images.begin(); it != _images.end(); it++) {
		free(it->second.first); //Free del header
		free(it->second.second);//Free de la imagen
	}
	_images.clear();
}

Resources * const Resources::GetInstance()
{
	if (_instance == nullptr)
		_instance = new Resources();

	return _instance;
}

void Resources::Release()
{
	if (_instance != nullptr) {
		delete _instance; _instance = nullptr;
	}
}

pair<uint32_t*, uint32_t*> Resources::getImage(ImageId id)
{

	map<ImageId, pair<uint32_t*, uint32_t*>>::iterator it = _images.find(id);

	if(it != _images.end())
		return (*it).second;

	else return {};
}

void Resources::load()
{
	for (int i = 0; i < _imageFiles.size(); i++) {
		std::string filePath = HEADER_PATH;	
		filePath.append(_imageFiles[i]);
		uint32_t* header = (uint32_t*)malloc(sizeof(uint32_t) * 2);

		FILE* ptr = BinReader::OpenFile(filePath.c_str());
		BinReader::Read(header, sizeof(uint32_t), 2, ptr);
		header = Platform::ToCurrentEndian(header, 2);

		uint32_t* buffer = (uint32_t*)malloc(sizeof(uint32_t) * header[0] * header[1]);
		BinReader::Read(buffer, sizeof(uint32_t), header[0] * header[1], ptr);
		buffer = Platform::ToCurrentEndian(buffer, header[0]* header[1]);

		_images.insert({ImageId(i), { header, buffer }});

		BinReader::CloseFile(ptr);
	}
}
