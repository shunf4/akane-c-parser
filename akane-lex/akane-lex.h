#pragma once

#include "..\akane-lang\akane-lang.h"
#include <fstream>

DEFINE_AKANE_EXCEPTION(LexAnalyze)

namespace AkaneLex {

	extern const std::set<std::string> keywordSet;
	extern const std::set<std::string> operatorSet;
	extern const std::set<std::string> delimiterSet;

	template <class L, template <class LL> class AutomataState>
	class LexDFA : public AkaneLang::DFA<L, AutomataState>
	{
	public:
		LexDFA(const std::vector< L> &_alphabet, const std::map<L, L> &_tagifyMap, AkaneLang::LetterGenerator *_letterGenP) : AkaneLang::DFA<L, AutomataState>(_alphabet, _tagifyMap, _letterGenP) { this->AkaneLang::DFA<L, AutomataState>::DFA(_alphabet, _tagifyMap, _letterGenP); }
		LexDFA(AkaneLang::NFA<L, AutomataState> &nfa) : AkaneLang::DFA<L, AutomataState>(nfa) {
			if (this->initialStateIndex == AkaneLang::invalidStateIndex)
				this->AkaneLang::DFA<L, AutomataState>::DFA(nfa);
		}
		virtual std::string typeStr() { return "Lexical DFA"; }

		AkaneLang::Token (*getToken)(std::istringstream &stream);
	};

	class LexicalAnalyzer
	{
	public:
		//std::vector< AkaneLang::DFA<AkaneLang::LetterString, AkaneLang::SimpleState> *> dfaPointers;
		std::istream *streamPointer;
		AkaneLang::LetterStringGenerator lsg;
		std::vector< AkaneLang::LetterToken> output;

		LexicalAnalyzer(std::istream *streamPointer);
		AkaneLex::LexDFA<AkaneLang::LetterString, AkaneLang::SimpleState> makeDFA(std::istream &is, const std::string &dfaNamePrefix, AkaneLang::Token(*getToken)(std::istringstream &stream));
		void analyze();
		std::vector< AkaneLang::LetterToken> getOutput();

		
		~LexicalAnalyzer();
	};
}