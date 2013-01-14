#ifndef READER_H_W1SG4X9M
#define READER_H_W1SG4X9M

#include <iostream>
#include "exception/readException.h"

typedef unsigned char byte;
typedef unsigned char * byteArray;

class CReader {
	protected:
		FILE * _file;
	public:
		char myChar;
		CReader(const char * fileName);
		~CReader();
		byteArray read(size_t length) const throw(CReadException);
};

#endif /* end of include guard: READER_H_W1SG4X9M */