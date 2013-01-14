#include "reader.h"

CReader::CReader(const char * fileName){
	this->_file = fopen(fileName,"r");
}

byteArray CReader::read(size_t length) const throw(CReadException){
	byteArray buffer = (byteArray)calloc(sizeof(byte),length);
	if(fread(buffer,1,length,this->_file) != length){
		throw CReadException("Unable to read bytes.");
	}
	return buffer;
}

CReader::~CReader(){
	fclose(this->_file);
}