#include "readException.h"

CReadException::CReadException(std::string message){
	this->message = message;
}

const char * CReadException::what() const throw(){
	return this->message.c_str();
}

CReadException::~CReadException() throw (){}