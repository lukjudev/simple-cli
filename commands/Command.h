#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>

#include "CommandSegment.h"

class Command {
public:
	virtual ~Command() {};
	virtual void execute(std::istream& in, std::ostream& out, std::ostream& err) = 0;
protected:
	explicit Command(CommandSegment s) : segment(s) {};
	CommandSegment segment;
};

#endif