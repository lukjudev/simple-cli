#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "CommandFactory.h"
#include "Interpreter.h"
#include "InterpreterErrors.h"
#include "Lexer.h"
#include "Parser.h"

void Interpreter::run() {
	std::string line;
	while (true) {
		std::cout << prompt << " ";
		if (!std::getline(std::cin, line)) {
			break;
		}
		if (line.size() > charLimit) {
			line = line.substr(0, charLimit);
		}

		processLine(line, std::cout, std::cerr);

		if (std::cin.eof() || std::cin.fail()) {
			std::cin.clear();
		}
	}
}

void Interpreter::processLine(const std::string& line) {
	processLine(line, std::cout, std::cerr);
}

// ekstenzija sa out i err zbog batch komande (sve komande unutar batch-a treba da koriste njegov outstream)
void Interpreter::processLine(const std::string& line, std::ostream& out, std::ostream& err) {
	try {
		if (line.empty()) {
			return;
		}

		Lexer lexer;
		const std::vector<Token> tokens = lexer.tokenize(line);

		Parser parser;
		const Pipeline pl = parser.parseLine(tokens);

		executePipeline(pl, out, err);
	} catch (const InterpreterError& ex) {
		err << ex.what() << '\n';
	} catch (const std::exception& ex) {
		err << ex.what() << '\n';
	}

	std::cin.clear();
}

void Interpreter::executePipeline(const Pipeline& pl, std::ostream& out, std::ostream& err) {
	if (pl.segments.empty()) {
		throw SyntaxError("Sintaksna greska: nema komande");
	}

	// samo jedna komanda u pipe-u
	// gin - global input, gout -  global output
	// po defaultu su cin i cout (prosledjen kao argument), ali mogu da se promene u zavisnosti od argumenta
	if (pl.segments.size() == 1) {
		std::ifstream gin;
		std::istream* inPtr = &std::cin;
		// ako postoji inStream, koristimo njega umesto cin
		if (!pl.inStream.empty()) {
			gin.open(pl.inStream);
			if (!gin) {
				throw CommandExecutionError("Greska: nije moguce otvoriti ulaznu datoteku '" + pl.inStream + "'");
			}
			inPtr = &gin;
		}

		std::ofstream gout;
		std::ostream* outPtr = &out;
		// ako postoji outStream, koristimo njega umesto cout
		if (!pl.outStream.empty()) {
			// append ili truncate (> ili >>)
			const auto mode = pl.appendOutStream ? (std::ios::app) : (std::ios::trunc);
			gout.open(pl.outStream, mode);
			if (!gout) {
				throw CommandExecutionError("Greska: nije moguce otvoriti izlaznu datoteku '" + pl.outStream + "'");
			}
			outPtr = &gout;
		}

		// treba nam pokazivac na interpreter jer ga batch i prompt koriste
		Command* cmd = CommandFactory::create(pl.segments[0], *this);
		cmd->execute(*inPtr, *outPtr, err);
		delete cmd;
		return;
	}

	// pipeline sa vise od jedne komande
	std::string buffer;
	std::ifstream gin;
	std::istream* firstIn = &std::cin;
	if (!pl.inStream.empty()) {
		gin.open(pl.inStream);
		if (!gin) {
			throw CommandExecutionError("Greska: nije moguce otvoriti ulaznu datoteku '" + pl.inStream + "'");
		}
		firstIn = &gin;
	}

	// stringstream simulira "medjukorake" u pipeline-u
	for (std::size_t i = 0; i < pl.segments.size(); i++) {
		std::stringstream pipeIn;
		std::stringstream pipeOut;
		std::istream* inPtr = nullptr;
		if (i == 0) {
			inPtr = firstIn;
		} else {
			pipeIn.str(buffer);
			pipeIn.clear();
			inPtr = &pipeIn;
		}

		std::ostream* outPtr = nullptr;
		std::ofstream fileOut;
		// ako si na poslednjem segmentu ispisuj u outstream, a u suprotnom u stringstream
		if (i == pl.segments.size() - 1) {
			if (!pl.outStream.empty()) {
				const auto mode = pl.appendOutStream ? (std::ios::app) : (std::ios::trunc);
				fileOut.open(pl.outStream, mode);
				if (!fileOut) {
					throw CommandExecutionError("Greska: nije moguce otvoriti izlaznu datoteku '" + pl.outStream + "'");
				}
				outPtr = &fileOut;
			} else {
				outPtr = &out;
			}
		} else {
			outPtr = &pipeOut;
		}

		Command* cmd = CommandFactory::create(pl.segments[i], *this);
		cmd->execute(*inPtr, *outPtr, err);
		delete cmd;
		if (i < pl.segments.size() - 1) {
			buffer = pipeOut.str();
		}
	}
}
