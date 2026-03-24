#ifndef TOUCH_COMMAND_H
#define TOUCH_COMMAND_H

#include "Command.h"
#include "CommandSegment.h"

class TouchCommand : public Command {
public:
	explicit TouchCommand(CommandSegment segment);
	virtual void execute(std::istream& in, std::ostream& out, std::ostream& err) override;
};

#endif
