#ifndef UNKNOWN_COMMAND_ERROR_H
#define UNKNOWN_COMMAND_ERROR_H

#include <string>

#include "InterpreterError.h"

class UnknownCommandError : public InterpreterError {
public:
	explicit UnknownCommandError(const std::string& commandName) : InterpreterError("Nepoznata komanda: " + commandName) {}
};

#endif
