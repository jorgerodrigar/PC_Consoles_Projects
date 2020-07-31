#pragma once
#include <stdio.h>

///clase lectora de archivos binarios que facilita la abstraccion en otras clases. 
class BinReader
{
private:
	BinReader();
	~BinReader();

public:
	///abre un archivo en modo lectura binaria y devuelve el puntero al mismo
	static FILE* OpenFile(const char* filePath);
	///cierra el archivo especificado por filePtr
	static void CloseFile(FILE* filePtr);
	///lee y devuelve un solo caracter
	static int ReadByte(FILE* filePtr);
	///lee size bytes del archivo especificado por filePtr y los almacena en buffer
	static void Read(void* buffer, size_t size, size_t nMemb, FILE* filePtr);
};

