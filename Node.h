//
// Created by yoon on 3/16/25.
//

#ifndef KORLANG_NODE_H
#define KORLANG_NODE_H

#include "Kind.h"

struct Statement {};
struct Expression {};

typedef struct Function : Statement {
    wstring name;
    vector<wstring> parameters;
    vector<Statement*> blocks;
} Function;

struct Program {
    vector<Function*> functions;
};

struct Return : Statement {
    Expression* expression;
};

struct Variable : Statement {
    wstring name;
    Expression* expression;
};

struct For : Statement {
    Variable* variable;
    Expression* condition;
    Expression* expression;
    vector<Statement*> blocks;
};

struct Break : Statement {};
struct Continue : Statement {};

struct If : Statement {
    vector<Expression*> conditions;
	vector<vector<Statement*>> blocks; // if, elif, else 각각의 블록
    vector<Statement*> elseBlock;
};

struct Print : Statement {
    vector<Expression*> arguments;
};

struct ExpressionStatement : Statement {
    Expression* expression;
};

struct Or : Expression {
    Expression* lhs;
    Expression* rhs;
};

struct And : Expression {
    Expression* lhs;
    Expression* rhs;
};

struct Relational : Expression {
    Kind kind;
    Expression* lhs;
    Expression* rhs;
};

struct Arithmetic : Expression {
    Kind kind;
    Expression* lhs;
    Expression* rhs;
};

struct Unary : Expression {
    Kind kind;
    Expression* sub;
};

struct Call : Expression {
    Expression* sub;
    vector<Expression*> arguments;
};

// 배열, map 원소의 참조
// arr[0]
struct GetElement : Expression {
    Expression* sub;
    Expression* index;
};

// arr[0] = 3;
// 할당은 statement 아닌가?
struct SetElement : Expression {
    Expression* sub;
    Expression* index;
    Expression* value;
};

// 변수 참조
struct GetVariable : Expression {
    wstring name;
};

// 변수 수정
struct SetVariable : Expression {
    wstring name;
    Expression* value;
};

struct NullLiteral : Expression {};
struct BooleanLiteral : Expression {
    bool value = false;
};
struct NumberLiteral : Expression {
    double value = 0.0;
};
struct StringLiteral : Expression {
    wstring value;
};
struct ArrayLiteral : Expression {
    vector<Expression*> values;
};
struct MapLiteral : Expression {
    map<wstring, Expression*> values;
};
#endif //KORLANG_NODE_H