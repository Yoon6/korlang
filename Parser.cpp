#include <vector>

#include "Node.h"
#include "Token.h"

static vector<Token>::iterator currentToken;

bool skipCurrentIf(Kind kind)
{
	if (currentToken->kind != kind)
	{
		return false;
	}

	currentToken++;
	return true;
}

void skipCurrent(Kind kind)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	if (currentToken->kind != kind)
	{
		cout << converter.to_bytes(toString(kind)) << " 토큰이 필요합니다.";
		exit(1);
	}
	currentToken++;
}


Expression* parseExpression()
{
	auto result = new Expression();

	return result;
}

Variable* parseVariable()
{
	auto result = new Variable();
	// `변수` 키워드를 건너뛴다.
	skipCurrent(Kind::Variable);

	result->name = currentToken->string;
	// skip variable name
	skipCurrent(Kind::Identifier);

	// skip =
	skipCurrent(Kind::Assignment);

	result->expression = parseExpression();

	// skip ;
	skipCurrent(Kind::Semicolon);

	return result;
}

For* parseFor()
{
	auto result = new For();

	// skip '반복'
	skipCurrent(Kind::For);


	return result;
}

If* parseIf()
{
	auto result = new If();

	return result;
}

Print* parsePrint()
{
	auto result = new Print();

	return result;
}

Return* parseReturn()
{
	auto result = new Return();

	return result;
}

Break* parseBreak()
{
	auto result = new Break();

	return result;
}

Continue* parseContinue()
{
	auto result = new Continue();

	return result;
}

ExpressionStatement* parseExpressionStatement()
{
	auto result = new ExpressionStatement();
	result->expression = parseExpression();
	skipCurrent(Kind::Semicolon);
	return result;
}

vector<Statement*> parseBlock()
{
	vector<Statement*> result;

	while (currentToken->kind != Kind::RightBrace)
	{
		switch (currentToken->kind)
		{
		case Kind::Variable:
			result.push_back(parseVariable());
			break;
		case Kind::For:
			result.push_back(parseFor());
			break;
		case Kind::If:
			result.push_back(parseIf());
			break;
		case Kind::Print:
			result.push_back(parsePrint());
			break;
		case Kind::Return:
			result.push_back(parseReturn());
			break;
		case Kind::Break:
			result.push_back(parseBreak());
			break;
		case Kind::Continue:
			result.push_back(parseContinue());
			break;
		case Kind::EndOfLine:
			cout << *currentToken << " 잘못된 구문입니다.";
			exit(1);
		default:
			result.push_back(parseExpressionStatement());
			break;
		}
	}

	return result;
}

Function* parseFunction()
{
	auto result = new Function();
	skipCurrent(Kind::Function);

	// function name
	result->name = currentToken->string;
	skipCurrent(Kind::Identifier);

	// (
	skipCurrent(Kind::LeftParen);

	// parameters
	if (currentToken->kind != Kind::RightParen)
	{
		do
		{
			result->parameters.push_back(currentToken->string);
			skipCurrent(Kind::Identifier);
		}
		while (skipCurrentIf(Kind::Comma));
	}

	// )
	skipCurrent(Kind::RightParen);
	// {
	skipCurrent(Kind::LeftBrace);
	// body
	result->blocks = parseBlock();
	// }
	skipCurrent(Kind::RightBrace);

	return result;
}

Program* parse(vector<Token> tokenList) {
	auto result = new Program();

	currentToken = tokenList.begin();

	while (currentToken->kind != Kind::EndOfLine)
	{
		switch (currentToken->kind)
		{
		case Kind::Function:
			result->functions.push_back(parseFunction());
			break;
		default:
			cout << *currentToken << " 잘못된 구문입니다.";
			exit(1);
		}
	}


	return result;
}