#include "EndianConverter.h"
#include <SDL.h>



EndianConverter::EndianConverter()
{
}


EndianConverter::~EndianConverter()
{
}

uint32_t * EndianConverter::toCurrentEndian(uint32_t * endian, uint32_t size)
{
#ifdef __ORBIS__
	return _toCurrentEndian(endian, size);

#elif _WIN64 || _WIN32
	if (SDL_BYTEORDER == SDL_LIL_ENDIAN)
		return _toCurrentEndian(endian, size);
	else
		return endian;

#endif
}

uint32_t * EndianConverter::_toCurrentEndian(uint32_t * endian, uint32_t size)
{
	for (int j = 0; j < size; j++)
		endian[j] = ((endian[j] >> 24) & 0x000000ff | (endian[j] >> 8) & 0x0000ff00 | (endian[j] << 8) & 0x00ff0000 | (endian[j] << 24) & 0xff000000);

	return endian;
}
