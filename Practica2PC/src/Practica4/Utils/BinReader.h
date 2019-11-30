#pragma once
#include <string>

class BinReader
{
private:
	BinReader();
	~BinReader();

public:
	static FILE* openFile(const char* filePath);
	static void closeFile(FILE* filePtr);
	static int readByte(FILE* filePtr);
	static void read(void* buffer, unsigned int size, FILE* filePtr);
};

