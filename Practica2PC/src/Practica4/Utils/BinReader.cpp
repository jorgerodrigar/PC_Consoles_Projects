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

void BinReader::Read(void* buffer, size_t size, size_t nMemb, FILE * filePtr)
{
	fread(buffer, size, nMemb, filePtr);
}
