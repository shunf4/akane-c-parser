#include "pch.h"
#include "..\akane-lex\akane-lex.h"
#include "..\akane-lang\akane-lang.h"
#include "..\akane-parse\akane-parse.h"
#include <iostream>
using namespace std;


void main0()
{
	
	ifstream f("test-text.txt", ios::in);
	AkaneLex::LexicalAnalyzer lex(&f);
	lex.analyze();
}

int main()
{
	AkaneUtils::globalLogFileName = "logs/akane-log-new-";
	AkaneUtils::globalLogFileName += AkaneUtils::getTimeString("%m%d%H%M%S");
	AkaneUtils::globalLogFileName += ".log";
	AkaneUtils::globalErrorFileName = "logs/akane-log-new-";
	AkaneUtils::globalErrorFileName += AkaneUtils::getTimeString("%m%d%H%M%S");
	AkaneUtils::globalErrorFileName += ".err.log";
	logger.printErrorToStderr = false;
	logger.printLogToStdout = false;

	cout << "正在词法分析..." << endl;

	// 词法分析
	ifstream f("test-text.txt", ios::in);
	AkaneLex::LexicalAnalyzer lex(&f);
	lex.analyze();
	auto lexOutput = lex.getOutput();

	cout << "正在语法分析..." << endl;

	// 语法分析
	AkaneLang::LetterTokenGenerator ltg = AkaneLang::LetterTokenGenerator(lexOutput);
	AkaneParse::Parser parser = AkaneParse::Parser(ltg);
	parser.analyze();

	cout << "词法和语法分析完成. 请参考 " << AkaneUtils::globalLogFileName << " 和 " << AkaneUtils::globalErrorFileName << " 获取详细日志." << endl;
}
