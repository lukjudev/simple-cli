#include <cstdio>
#include <vector>

#include "Command.h"
#include "InterpreterErrors.h"
#include "RmCommand.h"

RmCommand::RmCommand(CommandSegment segment) : Command(segment) {
	const std::vector<Token>& tokens = segment.tokens;
	const std::size_t n = tokens.size();

	if (tokens.empty()) {
		throw SyntaxError("Sintaksna greska: neispravan format komande rm");
	}

	if (n != 2) {
		throw SyntaxError("Sintaksna greska: neispravan format komande rm");
	}

	if (tokens[0].type != Normal || tokens[0].text != "rm") {
		throw SyntaxError("Sintaksna greska: neispravan format komande rm");
	}

	if (tokens[1].type != Normal) {
		throw SyntaxError("Sintaksna greska: neispravan format komande rm");
	}
}

void RmCommand::execute(std::istream& in, std::ostream& out, std::ostream& err) {
	const std::string& path = segment.tokens[1].text;
	//path.c_str() vraca char*
	if (std::remove(path.c_str()) != 0) {
		throw CommandExecutionError("Greska: nije moguce ukloniti datoteku '" + path + "'");
	}
}
