#ifndef LEXICAL_ERROR_H
#define LEXICAL_ERROR_H

#include <string>

#include "InterpreterError.h"

class LexicalError : public InterpreterError {
public:
	explicit LexicalError(std::string message) : InterpreterError(message) {}
};

#endif
