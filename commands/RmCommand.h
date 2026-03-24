#ifndef RM_COMMAND_H
#define RM_COMMAND_H

#include "Command.h"
#include "CommandSegment.h"

class RmCommand : public Command {
public:
	explicit RmCommand(CommandSegment segment);
	virtual void execute(std::istream& in, std::ostream& out, std::ostream& err) override;
};

#endif
