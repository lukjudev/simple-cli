#include <string>
#include <vector>

#include "InterpreterErrors.h"
#include "Parser.h"

// komanda uzima ulaz iz argumenta
bool stdinFromArgument(const CommandSegment& s) {
	if (s.tokens.empty() || s.tokens[0].type != Normal) {
		return false;
	}
	const std::string& cmd = s.tokens[0].text;

	if (cmd == "echo") {
		return s.tokens.size() >= 2;
	}
	if (cmd == "wc") {
		return s.tokens.size() >= 3;
	}
	if (cmd == "tr") {
		for (std::size_t i = 1; i + 1 < s.tokens.size(); i++) {
			const bool dashAndQuoted = (s.tokens[i].type == Normal && s.tokens[i].text == "-" && i < s.tokens.size() - 1 && s.tokens[i + 1].type == Quoted);
			if (dashAndQuoted) {
				return i > 1;
			}
		}
		return false;
	}
	if (cmd == "head") {
		return s.tokens.size() >= 3;
	}
	return false;
}

// ove komande moraju poslednje posto nemaju klasican izlaz 
bool mustBeLast(const std::string& cmd) {
	return cmd == "touch" || cmd == "rm" || cmd == "truncate" || cmd == "prompt";
}

// deli ceo vektor tokena na segmente izmedju |
std::vector<CommandSegment> Parser::splitLine(const std::vector<Token>& tokens) {
	std::vector<CommandSegment> segments;
	std::vector<Token> segment;

	const std::size_t n = tokens.size();

	for (std::size_t i = 0; i < n; i++) {
		if (tokens[i].type == Pipe) {
			segments.push_back(CommandSegment{ segment });
			segment.clear();
		} else {
			segment.push_back(tokens[i]);
		}
	}

	if (!segment.empty()) {
		segments.push_back(CommandSegment{ segment });
	}

	return segments;
}

// proverava da li je ceo pipeline ispravan (formati pojedinacnih komandi se proveravaju u njihovim konstruktorima)
void Parser::validateLine(const std::vector<CommandSegment>& segments) {
	if (segments.empty()) {
		throw SyntaxError("Sintaksna greska: nema komande");
	}

	// time i date mogu biti samo na prvom mestu
	for (std::size_t i = 0; i < segments.size(); i++) {
		const CommandSegment& seg = segments[i];

		if (seg.tokens.empty()) {
			throw SyntaxError("Sintaksna greska: prazan segment u cevovodu");
		}
		if (seg.tokens[0].type != Normal) {
			throw SyntaxError("Sintaksna greska: naziv komande mora biti jedna rec bez navodnika");
		}

		const std::string& cmd = seg.tokens[0].text;

		if (i > 0 && (cmd == "time" || cmd == "date")) {
			throw SemanticError("Semanticka greska: komande time i date smeju biti samo prve u cevovodu");
		}
		if (mustBeLast(cmd) && i + 1 < segments.size()) {
			throw SemanticError("Semanticka greska: nedozvoljeni raspored komandi");
		}
		if (i > 0 && stdinFromArgument(seg)) {
			throw SemanticError("Semanticka greska: konflikt ulaza u cevovodu (komanda cita ulaz iz argumenta)");
		}
	}
}

Pipeline Parser::parseLine(const std::vector<Token>& tokens) {
	if (tokens.empty()) {
		throw SyntaxError("Sintaksna greska: prazna komandna linija");
	}

	Pipeline result;
	std::vector<Token> parsed = tokens;

	// skidanje redirekcija sa kraja
	while (parsed.size() >= 2) {
		const Token& redirectToken = parsed[parsed.size() - 2];
		const Token& pathToken = parsed[parsed.size() - 1];
		if (pathToken.type != Normal && pathToken.type != Quoted) {
			break;
		}
		const std::string fname = pathToken.text;

		if (redirectToken.type == RedirectIn) {
			if (!result.inStream.empty()) {
				throw SemanticError("Semanticka greska: dvostruka redirekcija ulaza");
			}
			result.inStream = fname;
			parsed.pop_back();
			parsed.pop_back();
			continue;
		}
		if (redirectToken.type == RedirectOut) {
			if (!result.outStream.empty()) {
				throw SemanticError("Semanticka greska: dvostruka redirekcija izlaza");
			}
			result.outStream = fname;
			result.appendOutStream = false;
			parsed.pop_back();
			parsed.pop_back();
			continue;
		}
		if (redirectToken.type == RedirectOutAppend) {
			if (!result.outStream.empty()) {
				throw SemanticError("Semanticka greska: dvostruka redirekcija izlaza");
			}
			result.outStream = fname;
			result.appendOutStream = true;
			parsed.pop_back();
			parsed.pop_back();
			continue;
		}
		break;
	}

	if (parsed.empty()) {
		throw SyntaxError("Sintaksna greska: nema komande (samo redirekcije)");
	}

	result.segments = splitLine(parsed);
	validateLine(result.segments);

	// ne moze globalni redirect < i koamnda sa svojim ulazom
	if (!result.inStream.empty() && !result.segments.empty() && stdinFromArgument(result.segments[0])) {
		throw SemanticError("Semanticka greska: konflikt izmedju redirekcije ulaza i argumenta komande");
	}

	return result;
}
