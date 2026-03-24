#include <fstream>
#include <string>
#include <vector>

#include "Command.h"
#include "InterpreterErrors.h"
#include "TouchCommand.h"

TouchCommand::TouchCommand(CommandSegment segment) : Command(segment) {
	const std::vector<Token>& tokens = segment.tokens;
	const std::size_t n = tokens.size();

	if (tokens.empty()) {
		throw SyntaxError("Sintaksna greska: neispravan format komande touch");
	}

	if (n != 2) {
		throw SyntaxError("Sintaksna greska: neispravan format komande touch");
	}

	if (tokens[0].type != Normal || tokens[0].text != "touch") {
		throw SyntaxError("Sintaksna greska: neispravan format komande touch");
	}

	if (tokens[1].type != Normal) {
		throw SyntaxError("Sintaksna greska: neispravan format komande touch");
	}
}

void TouchCommand::execute(std::istream& in, std::ostream& out, std::ostream& err) {
	const std::string& path = segment.tokens[1].text;

	std::ifstream exists(path);
	if (exists) {
		throw CommandExecutionError("Greska: datoteka '" + path + "' vec postoji");
	}

	std::ofstream file(path);
	if (!file) {
		throw CommandExecutionError("Greska: nije moguce kreirati datoteku '" + path + "'");
	}
}
