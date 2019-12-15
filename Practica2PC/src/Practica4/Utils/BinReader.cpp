#include "BinReader.h"

BinReader::BinReader()
{
}


BinReader::~BinReader()
{
}

FILE* BinReader::OpenFile(const char* filePath)
{
	return fopen(filePath, "rb");
}

void BinReader::CloseFile(FILE* filePtr)
{
	fclose(filePtr);
}

int BinReader::ReadByte(FILE* filePtr)
{
	return fgetc(filePtr);
}

void BinReader::Read(void* buffer, unsigned int size, FILE * filePtr)
{
	fread(buffer, sizeof(buffer), size, filePtr);
}
