#ifndef SEMANTIC_ERROR_H
#define SEMANTIC_ERROR_H

#include <string>

#include "InterpreterError.h"

class SemanticError : public InterpreterError {
public:
	explicit SemanticError(std::string message) : InterpreterError(message) {}
};

#endif
