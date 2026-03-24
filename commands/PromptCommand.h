#ifndef PROMPT_COMMAND_H
#define PROMPT_COMMAND_H

#include "Command.h"
#include "CommandSegment.h"

class Interpreter;

class PromptCommand : public Command {
private:
	Interpreter& interpreter;

public:
	PromptCommand(CommandSegment segment, Interpreter& interpreter);
	virtual void execute(std::istream& in, std::ostream& out, std::ostream& err) override;
};

#endif
