#include "BatchCommand.h"
#include "CommandFactory.h"
#include "DateCommand.h"
#include "EchoCommand.h"
#include "HeadCommand.h"
#include "InterpreterErrors.h"
#include "PromptCommand.h"
#include "RmCommand.h"
#include "TimeCommand.h"
#include "TouchCommand.h"
#include "TruncateCommand.h"
#include "TrCommand.h"
#include "WcCommand.h"

Command* CommandFactory::create(const CommandSegment& segment, Interpreter& interpreter) {
	if (segment.tokens.empty() || segment.tokens[0].type != Normal) {
		throw SyntaxError("Sintaksna greska: naziv komande mora biti jedna rec bez navodnika");
	}

	const std::string& name = segment.tokens[0].text;

	if (name == "echo") {
		return new EchoCommand(segment);
	}
	if (name == "time") {
		return new TimeCommand(segment);
	}
	if (name == "date") {
		return new DateCommand(segment);
	}
	if (name == "touch") {
		return new TouchCommand(segment);
	}
	if (name == "truncate") {
		return new TruncateCommand(segment);
	}
	if (name == "rm") {
		return new RmCommand(segment);
	}
	if (name == "wc") {
		return new WcCommand(segment);
	}
	if (name == "tr") {
		return new TrCommand(segment);
	}
	if (name == "head") {
		return new HeadCommand(segment);
	}
	if (name == "prompt") {
		return new PromptCommand(segment, interpreter);
	}
	if (name == "batch") {
		return new BatchCommand(segment, interpreter);
	}

	throw UnknownCommandError(name);
}
