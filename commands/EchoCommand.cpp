#include <fstream>
#include <string>

#include "Command.h"
#include "EchoCommand.h"
#include "InterpreterErrors.h"

EchoCommand::EchoCommand(CommandSegment segment) : Command(segment) {
	const std::vector<Token>& tokens = segment.tokens;
	const std::size_t n = tokens.size();

	if (tokens.empty()) {
		throw SyntaxError("Sintaksna greska: neispravan format komande echo");
	}
	if (tokens[0].type != Normal || tokens[0].text != "echo") {
		throw SyntaxError("Sintaksna greska: neispravan format komande echo");
	}
	if (n == 2 && tokens[1].type != Normal && tokens[1].type != Quoted) {
		throw SyntaxError("Sintaksna greska: neispravan format komande echo");
	}
	if (n > 2) {
		throw SyntaxError("Sintaksna greska: neispravan format komande echo");
	}
}

void EchoCommand::execute(std::istream& in, std::ostream& out, std::ostream& err) {
	const std::vector<Token>& tokens = segment.tokens;

	if (tokens.size() == 1) {
		std::string content;
		std::string line;
		while (std::getline(in, line)) {
			content += line + '\n';
		}

		if (!content.empty() && content[content.size() - 1] == '\n') {
			content = content.substr(0, content.size() - 1);
		}
		out << content;

		if (&out == &std::cout) {
			out << '\n';
		}

		return;
	}

	const Token& arg = tokens[1];
	if (arg.type == Quoted) {
		out << arg.text;
		if (&out == &std::cout) {
			out << '\n';
		}
		return;
	}

	std::ifstream file(arg.text);
	if (!file) {
		throw CommandExecutionError("Greska: nije moguce otvoriti datoteku '" + arg.text + "'");
	}

	std::string fcontent;
	std::string fline;
	while (std::getline(file, fline)) {
		fcontent += fline + '\n';
	}
	if (!fcontent.empty() && fcontent[fcontent.size() - 1] == '\n') {
		fcontent = fcontent.substr(0, fcontent.size() - 1);
	}
	out << fcontent;
	if (&out == &std::cout) {
		out << '\n';
	}
}
