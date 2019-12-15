#pragma once
#include <stdio.h>

class BinReader
{
private:
	BinReader();
	~BinReader();

public:
	static FILE* OpenFile(const char* filePath);
	static void CloseFile(FILE* filePtr);
	static int ReadByte(FILE* filePtr);
	static void Read(void* buffer, unsigned int size, FILE* filePtr);
};

