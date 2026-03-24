#ifndef DATE_COMMAND_H
#define DATE_COMMAND_H

#include "Command.h"
#include "CommandSegment.h"

class DateCommand : public Command {
public:
	explicit DateCommand(CommandSegment segment);
	virtual void execute(std::istream& in, std::ostream& out, std::ostream& err) override;
};

#endif
