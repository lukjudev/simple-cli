#ifndef BATCH_COMMAND_H
#define BATCH_COMMAND_H

#include "Command.h"
#include "CommandSegment.h"

class Interpreter;

class BatchCommand : public Command {
private:
	Interpreter& interpreter;

public:
	BatchCommand(CommandSegment segment, Interpreter& interpreter);
	virtual void execute(std::istream& in, std::ostream& out, std::ostream& err) override;
};

#endif
