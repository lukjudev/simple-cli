#ifndef COMMAND_FACTORY_H
#define COMMAND_FACTORY_H

#include "Command.h"
#include "CommandSegment.h"

class Interpreter;

class CommandFactory {
public:
	static Command* create(const CommandSegment& segment, Interpreter& interpreter);
};

#endif
