#include <vector>

#include "Node.h"
#include "Token.h"

static vector<Token>::iterator currentToken;

vector<Statement*> parseBlock();

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

	result->variable = new Variable();
	result->variable->name = currentToken->string;

	// skip variable name
	skipCurrent(Kind::Identifier);
	// skip =
	skipCurrent(Kind::Assignment);
	result->variable->expression = parseExpression();

	if (result->variable->expression == nullptr)
	{
		cout << "for문에 초기화 식이 없습니다.";
		exit(1);
	}

	// skip ;
	skipCurrent(Kind::Semicolon);
	result->condition = parseExpression();
	if (result->condition == nullptr)
	{
		cout << "for문에 조건식이 없습니다.";
		exit(1);
	}

	// skip ;
	skipCurrent(Kind::Semicolon);
	result->expression = parseExpression();
	if (result->expression == nullptr)
	{
		cout << "for문에 증감식이 없습니다.";
		exit(1);
	}

	// skip {
	skipCurrent(Kind::LeftBrace);
	result->blocks = parseBlock();
	// skip }
	skipCurrent(Kind::RightBrace);

	return result;
}

If* parseIf()
{
	auto result = new If();
	
	// skip '만약'
	skipCurrent(Kind::If);
	do
	{
		auto condition = parseExpression();
		if (condition == nullptr)
		{
			cout << "if문에 조건식이 없습니다.";
			exit(1);
		}
		result->conditions.push_back(condition);
		// skip {
		skipCurrent(Kind::LeftBrace);
		result->blocks.push_back(parseBlock());
		// skip }
		skipCurrent(Kind::RightBrace);
	}
	while (skipCurrentIf(Kind::Elif));

	if (skipCurrentIf(Kind::Else))
	{
		// skip {
		skipCurrent(Kind::LeftBrace);
		result->elseBlock = parseBlock();
		// skip }
		skipCurrent(Kind::RightBrace);
	}

	return result;
}

Print* parsePrint()
{
	auto result = new Print();
	// skip '출력'
	skipCurrent(Kind::Print);
	// skip (
	skipCurrent(Kind::LeftParen);

	// 하나의 식만 출력 가능
	result->arguments.push_back(parseExpression());

	// skip )
	skipCurrent(Kind::RightParen);
	// skip ;
	skipCurrent(Kind::Semicolon);


	return result;
}

Return* parseReturn()
{
	auto result = new Return();

	// skip '반환'
	skipCurrent(Kind::Return);

	result->expression = parseExpression();
	if (result->expression == nullptr)
	{
		cout << "반환문에 반환값이 없습니다.";
		exit(1);
	}

	// skip ;
	skipCurrent(Kind::Semicolon);

	return result;
}

Break* parseBreak()
{
	auto result = new Break();
	// skip '끊기'
	skipCurrent(Kind::Break);
	// skip ;
	skipCurrent(Kind::Semicolon);

	return result;
}

Continue* parseContinue()
{
	auto result = new Continue();
	// skip '계속하기'
	skipCurrent(Kind::Continue);
	// skip ;
	skipCurrent(Kind::Semicolon);

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