#include <vector>

#include "Command.h"
#include "Interpreter.h"
#include "InterpreterErrors.h"
#include "PromptCommand.h"

PromptCommand::PromptCommand(CommandSegment segment, Interpreter& interpreter)
	: Command(segment), interpreter(interpreter) {
	const std::vector<Token>& tokens = segment.tokens;
	const std::size_t n = tokens.size();

	if (tokens.empty()) {
		throw SyntaxError("Sintaksna greska: neispravan format komande prompt");
	}

	if (n != 2) {
		throw SyntaxError("Sintaksna greska: neispravan format komande prompt");
	}

	if (tokens[0].type != Normal || tokens[0].text != "prompt") {
		throw SyntaxError("Sintaksna greska: neispravan format komande prompt");
	}

	if (tokens[1].type != Quoted) {
		throw SyntaxError("Sintaksna greska: neispravan format komande prompt");
	}
}

void PromptCommand::execute(std::istream& in, std::ostream& out, std::ostream& err) {
	interpreter.setPrompt(segment.tokens[1].text);
}
