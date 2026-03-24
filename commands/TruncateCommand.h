#ifndef TRUNCATE_COMMAND_H
#define TRUNCATE_COMMAND_H

#include "Command.h"
#include "CommandSegment.h"

class TruncateCommand : public Command {
public:
	explicit TruncateCommand(CommandSegment segment);
	virtual void execute(std::istream& in, std::ostream& out, std::ostream& err) override;
};

#endif
