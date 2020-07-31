#pragma once

#include <stdint.h>
#include <map>
#include <vector>

using namespace std;

///clase manejadora de recursos. Singleton que carga y almacena en su inicializacion todos los recursos especificados.
class Resources
{
public:
	static Resources* const GetInstance();
	static void Release();

	///identificadores de las imagenes cargadas
	enum ImageId {
		debug2x2,
		debug500x500,
		bang, 
		cliente,
		dolares, 
		ladron,
		marcoPuerta, ///pedro puerta
		puertas
	};

	///devuelve un par, si existe, de la imagen especificada por id (first -> header (header[0]: width, header[1]: height), second -> image)
	pair<uint32_t*, uint32_t*> getImage(ImageId id);

private:
	static Resources* _instance;
	map<ImageId, pair<uint32_t*, uint32_t*>> _images;

	static std::vector<std::string> _imageFiles; // initialized in .cpp

	Resources();
	~Resources();

	///carga todas las imagenes especificadas por _imageFiles usando la clase BinReader y las almacena en un map(ImageId, pair(header, image))
	void load();
};

