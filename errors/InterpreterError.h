#ifndef INTERPRETER_ERROR_H
#define INTERPRETER_ERROR_H

#include <exception>
#include <string>

class InterpreterError : public std::exception {
public:
	explicit InterpreterError(std::string message) : message(message) {}
	const char* what() const noexcept override { return message.c_str(); }

protected:
	std::string message;
};

#endif
