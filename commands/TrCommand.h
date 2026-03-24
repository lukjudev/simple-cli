#ifndef TR_COMMAND_H
#define TR_COMMAND_H

#include <cstddef>

#include "Command.h"
#include "CommandSegment.h"

class TrCommand : public Command {
private:
	std::size_t dashPos;

public:
	explicit TrCommand(CommandSegment segment);
	void execute(std::istream& in, std::ostream& out, std::ostream& err) override;
};

#endif
