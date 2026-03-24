#ifndef COMMAND_EXECUTION_ERROR_H
#define COMMAND_EXECUTION_ERROR_H

#include <string>

#include "InterpreterError.h"

class CommandExecutionError : public InterpreterError {
public:
	explicit CommandExecutionError(std::string message) : InterpreterError(message) {}
};

#endif
