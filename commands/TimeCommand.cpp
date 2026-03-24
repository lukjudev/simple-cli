#include <chrono>
#include <ctime>
#include <iomanip>
#include <vector>

#include "Command.h"
#include "InterpreterErrors.h"
#include "TimeCommand.h"

TimeCommand::TimeCommand(CommandSegment segment) : Command(segment) {
	const std::vector<Token> tokens = segment.tokens;
	const std::size_t n = tokens.size();

	if (tokens.empty()) {
		throw SyntaxError("Sintaksna greska: neispravan format komande time");
	}
	if (tokens[0].type != Normal || tokens[0].text != "time") {
		throw SyntaxError("Sintaksna greska: neispravan format komande time");
	}
	if (n > 1) {
		throw SyntaxError("Sintaksna greska: neispravan format komande time");
	}
}

void TimeCommand::execute(std::istream& in, std::ostream& out, std::ostream& err) {
	const auto now = std::chrono::system_clock::now();
	const std::time_t t = std::chrono::system_clock::to_time_t(now);
	std::tm local;
	if (localtime_s(&local, &t) != 0) {
		throw CommandExecutionError("Greska: nije moguce dobiti lokalno vreme");
	}
	out << std::put_time(&local, "%H:%M:%S") << '\n';
}
