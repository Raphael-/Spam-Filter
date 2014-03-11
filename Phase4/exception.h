#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <string>
#include <iostream>

using namespace std;

class IllegalNumberOfArgumentsException
{
public:
	const string message;
	const unsigned slot;
	IllegalNumberOfArgumentsException(const string& messageIn,unsigned s):
		message(messageIn), slot(s) {}
};

#endif /* EXCEPTION_H_ */
