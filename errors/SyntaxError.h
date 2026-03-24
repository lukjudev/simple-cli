#ifndef SYNTAX_ERROR_H
#define SYNTAX_ERROR_H

#include <string>

#include "InterpreterError.h"

class SyntaxError : public InterpreterError {
public:
	explicit SyntaxError(std::string message) : InterpreterError(message) {}
};

#endif
