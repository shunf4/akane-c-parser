#include "pch.h"
#include "..\akane-lex\c-style-lex.h"
#include "..\akane-lang\akane-lang.h"
#include "..\akane-parse\akane-parse.h"
#include <iostream>
#include <io.h>
#include <direct.h>
using namespace std;
using namespace AkaneParse;

DEFINE_AKANE_EXCEPTION(DllLoad)

void mkdirs(const char *muldir)
{
	size_t i, len;
	char str[512];
	strncpy(str, muldir, 512);
	len = strlen(str);
	for (i = 0; i < len; i++)
	{
		if (str[i] == '/')
		{
			str[i] = '\0';
			if (_access(str, 0) != 0)
			{
				_mkdir(str);
			}
			str[i] = '/';
		}
	}
	if (len > 0 && _access(str, 0) != 0)
	{
		_mkdir(str);
	}
	return;
}

int main()
{
	typedef void (*ImportedLexAnalyzeFunc)(TokenizedLetterGenerator &, StreamedLetterGenerator &);

	mkdirs("logs/");
	AkaneUtils::globalLogFileName = "logs/akane-log-new-";
	AkaneUtils::globalLogFileName += AkaneUtils::getTimeString("%m%d%H%M%S");
	AkaneUtils::globalLogFileName += ".log";
	AkaneUtils::globalErrorFileName = "logs/akane-log-new-";
	AkaneUtils::globalErrorFileName += AkaneUtils::getTimeString("%m%d%H%M%S");
	AkaneUtils::globalErrorFileName += ".err.log";
	logger.printErrorToStderr = false;
	logger.printLogToStdout = false;

	HMODULE lexDLL = LoadLibrary(TEXT("CSTYLELEX.dll"));
	if (!lexDLL)
	{
		throw AkaneDllLoadException("");
	}

	cout << "正在词法分析..." << endl;

	// 词法分析
	ImportedLexAnalyzeFunc lexFunc = (ImportedLexAnalyzeFunc)GetProcAddress(lexDLL, "?importedLexAnalyze@@YAXAEAUTokenizedLetterGenerator@AkaneLang@@AEAUStreamedLetterGenerator@2@@Z");

	ifstream finput("test-text.txt", ios::in);
	StreamedLetterGenerator input(finput);
	TokenizedLetterGenerator lexOutput;
	lexFunc(lexOutput, input);

	cout << "正在语法分析..." << endl;

	// 语法分析
	ifstream fgrammar("grammar.txt", ios::in);
	LR1Parser parser(fgrammar, lexOutput);
	parser.analyze();

	cout << "词法和语法分析完成. 请参考 " << AkaneUtils::globalLogFileName << " 和 " << AkaneUtils::globalErrorFileName << " 获取详细日志." << endl;
}
