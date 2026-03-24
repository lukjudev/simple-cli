#ifndef TIME_COMMAND_H
#define TIME_COMMAND_H

#include "Command.h"
#include "CommandSegment.h"

class TimeCommand : public Command {
public:
	explicit TimeCommand(CommandSegment segment);
	virtual void execute(std::istream& in, std::ostream& out, std::ostream& err) override;
};

#endif
