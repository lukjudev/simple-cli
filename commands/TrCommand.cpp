#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Command.h"
#include "InterpreterErrors.h"
#include "TrCommand.h"

namespace {

bool findDash(const std::vector<Token>& tokens, std::size_t& pos) {
	for (std::size_t i = 1; i + 1 < tokens.size(); i++) {
		if (tokens[i].type == Normal && tokens[i].text == "-" && tokens[i + 1].type == Quoted) {
			pos = i;
			return true;
		}
	}
	return false;
}

void replaceAll(std::string& s, const std::string& from, const std::string& to) {
	if (from.empty()) {
		return;
	}
	std::size_t pos = 0;
	while ((pos = s.find(from, pos)) != std::string::npos) {
		s.replace(pos, from.length(), to);
		pos += to.length();
	}
}

}

TrCommand::TrCommand(CommandSegment segment) : Command(segment), dashPos(0) {
	const std::vector<Token>& tokens = segment.tokens;
	const std::size_t n = tokens.size();

	if (tokens.empty()) {
		throw SyntaxError("Sintaksna greska: neispravan format komande tr");
	}

	if (tokens[0].type != Normal || tokens[0].text != "tr") {
		throw SyntaxError("Sintaksna greska: neispravan format komande tr");
	}

	if (n < 3) {
		throw SyntaxError("Sintaksna greska: neispravan format komande tr");
	}

	if (!findDash(tokens, dashPos)) {
		throw SyntaxError("Sintaksna greska: neispravan format komande tr");
	}

	if (tokens[dashPos + 1].text.empty()) {
		throw SyntaxError("Sintaksna greska: neispravan format komande tr");
	}

	if (dashPos == 1) {
		if (n == 3) {
			return;
		}
		if (n == 4 && tokens[3].type == Quoted) {
			return;
		}
		throw SyntaxError("Sintaksna greska: neispravan format komande tr");
	}

	if (dashPos == 2) {
		if (tokens[1].type != Normal && tokens[1].type != Quoted) {
			throw SyntaxError("Sintaksna greska: neispravan format komande tr");
		}
		if (n == 4) {
			return;
		}
		if (n == 5 && tokens[4].type == Quoted) {
			return;
		}
		throw SyntaxError("Sintaksna greska: neispravan format komande tr");
	}

	throw SyntaxError("Sintaksna greska: neispravan format komande tr");
}

void TrCommand::execute(std::istream& in, std::ostream& out, std::ostream& err) {
	const std::vector<Token>& tokens = segment.tokens;

	const std::string& searchFor = tokens[dashPos + 1].text;
	std::string replacement;
	if (dashPos + 2 < tokens.size() && tokens[dashPos + 2].type == Quoted) {
		replacement = tokens[dashPos + 2].text;
	}

	std::istream* src = &in;
	std::ifstream file;
	std::istringstream textIn;
	if (dashPos == 2) {
		if (tokens[1].type == Quoted) {
			textIn.str(tokens[1].text);
			textIn.clear();
			src = &textIn;
		} else {
			const std::string& path = tokens[1].text;
			file.open(path);
			if (!file) {
				throw CommandExecutionError("Greska: nije moguce otvoriti datoteku '" + path + "'");
			}
			src = &file;
		}
	}

	std::string content;
	std::string line;
	while (std::getline(*src, line)) {
		content += line + '\n';
	}
	if (!content.empty() && content[content.size() - 1] == '\n') {
		content = content.substr(0, content.size() - 1);
	}

	replaceAll(content, searchFor, replacement);
	out << content;
	if (&out == &std::cout) {
		out << '\n';
	}
}
