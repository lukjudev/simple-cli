#ifndef HEAD_COMMAND_H
#define HEAD_COMMAND_H

#include "Command.h"
#include "CommandSegment.h"

class HeadCommand : public Command {
public:
	explicit HeadCommand(CommandSegment segment);
	virtual void execute(std::istream& in, std::ostream& out, std::ostream& err) override;
};

#endif
