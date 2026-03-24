#ifndef ECHO_COMMAND_H
#define ECHO_COMMAND_H

#include "Command.h"
#include "CommandSegment.h"

class EchoCommand : public Command {
public:
	explicit EchoCommand(CommandSegment segment);
	virtual void execute(std::istream& in, std::ostream& out, std::ostream& err) override;

};

#endif