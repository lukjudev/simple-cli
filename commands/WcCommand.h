#ifndef WC_COMMAND_H
#define WC_COMMAND_H

#include "Command.h"
#include "CommandSegment.h"

class WcCommand : public Command {
public:
	explicit WcCommand(CommandSegment segment);
	virtual void execute(std::istream& in, std::ostream& out, std::ostream& err) override;
};

#endif
