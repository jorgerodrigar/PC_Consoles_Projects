#include "BinReader.h"

BinReader::BinReader()
{
}


BinReader::~BinReader()
{
}

FILE* BinReader::openFile(const char* filePath)
{
	return fopen(filePath, "rb");
}

void BinReader::closeFile(FILE* filePtr)
{
	fclose(filePtr);
}

int BinReader::readByte(FILE* filePtr)
{
	return fgetc(filePtr);
}

void BinReader::read(void* buffer, unsigned int size, FILE * filePtr)
{
	fread(buffer, sizeof(buffer), size, filePtr);
}
