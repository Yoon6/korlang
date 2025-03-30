#include <vector>
#include <set>

#include "Node.h"
#include "Token.h"

static vector<Token>::iterator currentToken;

vector<Statement*> parseBlock();
Expression* parseExpression();

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

Expression* parseElement(Expression* sub)
{
	auto result = new GetElement();
	result->sub = sub;

	// skip [
	skipCurrent(Kind::LeftBraket);

	// index 계산
	result->index = parseExpression();

	// skip ]
	skipCurrent(Kind::RightBraket);

	return result;
}

Expression* parseCall(Expression* sub)
{
	auto result = new Call();
	result->sub = sub;

	// skip (
	skipCurrent(Kind::LeftParen);

	// 파라미터 추가
	if (currentToken->kind != Kind::RightParen)
	{
		do
		{
			result->parameters.push_back(parseExpression());
		} while (skipCurrentIf(Kind::Comma));
	}
	// skip )
	skipCurrent(Kind::RightParen);
	return result;
}

Expression* parsePostfix(Expression* sub)
{
	while (true)
	{
		switch (currentToken->kind)
		{
		case Kind::LeftParen:
			sub = parseCall(sub);
			break;
		case Kind::LeftBraket:
			sub = parseElement(sub);
			break;
		default:
			return sub;
		}
	}
}


Expression* parseNullLiteral()
{
	skipCurrent(Kind::NullLiteral);
	auto result = new NullLiteral();
	return result;
}

Expression* parseBooleanLiteral()
{
	auto result = new BooleanLiteral();
	result->value = currentToken->kind == Kind::TrueLiteral;
	skipCurrent(currentToken->kind);
	return result;
}

Expression* parseNumberLiteral() 
{
	auto result = new NumberLiteral();
	result->value = stod(currentToken->string);
	skipCurrent(Kind::NumberLiteral);
	return result;
}

Expression* parseStringLiteral()
{
	auto result = new StringLiteral();
	result->value = currentToken->string;
	skipCurrent(Kind::StringLiteral);
	return result;
}

Expression* parseListLiteral()
{
	auto result = new ArrayLiteral();
	// skip [
	skipCurrent(Kind::LeftBraket);

	if (currentToken->kind != Kind::RightBraket)
	{
		do
		{
			result->values.push_back(parseExpression());
		} while (skipCurrentIf(Kind::Comma));
	}

	// skip ]
	skipCurrent(Kind::RightBraket);
	return result;
}

Expression* parseMapLiteral()
{
	auto result = new MapLiteral();
	// skip {
	skipCurrent(Kind::LeftBrace);

	if (currentToken->kind != Kind::RightBrace)
	{
		do
		{
			wstring name = currentToken->string;
			skipCurrent(Kind::StringLiteral);
			skipCurrent(Kind::Colon);

			auto value = parseExpression();
			result->values[name] = value;
		} while (skipCurrentIf(Kind::Comma));
	}

	// skip }
	skipCurrent(Kind::RightBrace);
	return result;
}

Expression* parseIdentifier() 
{
	auto result = new GetVariable();
	result->name = currentToken->string;
	skipCurrent(Kind::Identifier);
	return result;
}

Expression* parseInnerExpression()
{
	skipCurrent(Kind::LeftParen);
	auto result = parseExpression();
	skipCurrent(Kind::RightParen);
	return result;
}


Expression* parseOperand()
{
	Expression* result = nullptr;
	switch (currentToken->kind)
	{
	case Kind::NullLiteral:   result = parseNullLiteral();      break;
	case Kind::TrueLiteral:
	case Kind::FalseLiteral:  result = parseBooleanLiteral();   break;
	case Kind::NumberLiteral: result = parseNumberLiteral();    break;
	case Kind::StringLiteral: result = parseStringLiteral();    break;
	case Kind::LeftBraket:    result = parseListLiteral();      break;
	case Kind::LeftBrace:     result = parseMapLiteral();       break;
	case Kind::Identifier:    result = parseIdentifier();       break;
	case Kind::LeftParen:     result = parseInnerExpression();  break;
	default:                  cout << "잘못된 식입니다.";       exit(1);
	}

	return parsePostfix(result);
}

Expression* parseUnary()
{
	set<Kind> operators = {
		Kind::Add,
		Kind::Subtract,
	};
	while (operators.count(currentToken->kind))
	{
		auto temp = new Unary();
		temp->kind = currentToken->kind;
		skipCurrent(currentToken->kind);
		temp->sub = parseUnary();
		return temp;
	}

	return parseOperand();
}

Expression* parseArithmetic2()
{
	set<Kind> operators = {
	Kind::Multiply,
	Kind::Divide,
	Kind::Modulo,
	};
	auto result = parseUnary();

	while (operators.count(currentToken->kind))
	{
		auto temp = new Arithmetic();
		temp->kind = currentToken->kind;
		skipCurrent(currentToken->kind);
		temp->lhs = result;
		temp->rhs = parseUnary();
		result = temp;
	}

	return result;
}

Expression* parseArithmetic1()
{
	set<Kind> operators = {
		Kind::Add,
		Kind::Subtract,
	};
	auto result = parseArithmetic2();

	while (operators.count(currentToken->kind))
	{
		auto temp = new Arithmetic();
		temp->kind = currentToken->kind;
		skipCurrent(currentToken->kind);
		temp->lhs = result;
		temp->rhs = parseArithmetic2();
		result = temp;
	}

	return result;
}

Expression* parseRelational() // <, >, == ...
{
	set<Kind> operators = {
		Kind::Equal,
		Kind::NotEqual,
		Kind::LessThan,
		Kind::GreaterThan,
		Kind::LessOrEqual,
		Kind::GreaterOrEqual,
	};
	auto result = parseArithmetic1();

	while (operators.count(currentToken->kind))
	{
		auto temp = new Relational();
		temp->kind = currentToken->kind;
		skipCurrent(currentToken->kind);
		temp->lhs = result;
		temp->rhs = parseArithmetic1();
		result = temp;
	}

	return result;
}

Expression* parseAnd()
{
	auto result = parseRelational();

	while (skipCurrentIf(Kind::LogicalAnd)) 
	{
		auto temp = new And();
		temp->lhs = result;
		temp->rhs = parseRelational();
		result = temp; // { { parseEquality() && parseEquality() } && parseEquality() } && parseEquality() ...
	}

	return result;

}

Expression* parseOr()
{
	auto result = parseAnd();

	while (skipCurrentIf(Kind::LogicalOr))
	{
		auto temp = new Or();
		temp->lhs = result;
		temp->rhs = parseAnd();
		result = temp; // { { parseAnd() || parseAnd() } || parseAnd() } || parseAnd() ...
	}

	return result;
}


Expression* parseAssignment()
{
	// = 대입 연산자
	// 1. 왼쪽 피연산자부터 파싱
	Expression* result = parseOr();

	if (currentToken->kind != Kind::Assignment)
	{
		return result;
	}

	// skip =
	skipCurrent(Kind::Assignment);

	// GetVariable -> 참조 변수 타입
	GetVariable* variable = dynamic_cast<GetVariable*>(result);
	if (variable != nullptr)
	{
		SetVariable* result = new SetVariable();
		result->name = variable->name;
		result->value = parseAssignment();
		return result;
	}

	// GetElement -> 배열, map 원소 참조 타입
	GetElement* element = dynamic_cast<GetElement*>(result);
	if (element != nullptr)
	{
		SetElement* result = new SetElement();
		result->sub = element->sub;
		result->index = element->index;
		result->value = parseAssignment();
		return result;
	}

	cout << "대입 연산자의 왼쪽 피연산자가 변수나 배열, map 원소 참조가 아닙니다.";
	exit(1);
}

Expression* parseExpression()
{
	return parseAssignment();
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
    if (skipCurrentIf(Kind::PrintLine))
    {
        result->isPrintLine = true;
    }
    else
    {
        skipCurrent(Kind::Print);
        result->isPrintLine = false;
    }
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
        case Kind::PrintLine:
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