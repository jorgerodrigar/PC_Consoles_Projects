#pragma once
#include <iostream>

class EndianConverter
{
private:
	EndianConverter();
	~EndianConverter();
	static uint32_t* _toCurrentEndian(uint32_t* endian, uint32_t size);

public:
	static uint32_t* toCurrentEndian(uint32_t* endian, uint32_t size);

};

