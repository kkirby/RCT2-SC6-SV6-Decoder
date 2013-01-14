#ifndef READEXCEPTION_H_8I1F4SJL
#define READEXCEPTION_H_8I1F4SJL

#include <exception>
#include <string>

class CReadException : public std::exception {
	public:
		std::string message;
		CReadException(std::string message);
		virtual const char * what() const throw();
		virtual ~CReadException() throw ();
};	

#endif /* end of include guard: READEXCEPTION_H_8I1F4SJL */
