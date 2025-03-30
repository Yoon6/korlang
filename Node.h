//
// Created by yoon on 3/16/25.
//

#ifndef KORLANG_NODE_H
#define KORLANG_NODE_H

#include <vector>
#include "Kind.h"

struct Statement {
    virtual auto print(int) -> void = 0;
    virtual auto generate() -> void = 0;
};
struct Expression {
    virtual auto print(int) -> void = 0;
    virtual auto generate() -> void = 0;
};

typedef struct Function : Statement {
    wstring name;
    vector<wstring> parameters;
    vector<Statement*> blocks;
    auto print(int) -> void;
    auto generate() -> void;
} Function;

struct Program {
    vector<Function*> functions;
};

struct Return : Statement {
    Expression* expression;
    auto print(int) -> void;
    auto generate() -> void;
};

struct Variable : Statement {
    wstring name;
    Expression* expression;
    auto print(int) -> void;
    auto generate() -> void;
};

struct For : Statement {
    Variable* variable;
    Expression* condition;
    Expression* expression;
    vector<Statement*> blocks;
    auto print(int) -> void;
    auto generate() -> void;
};

struct Break : Statement {
    auto print(int) -> void;
    auto generate() -> void;
};
struct Continue : Statement {
    auto print(int) -> void;
    auto generate() -> void;
};

struct If : Statement {
    vector<Expression*> conditions;
	vector<vector<Statement*>> blocks; // if, elif, else 각각의 블록
    vector<Statement*> elseBlock;
    auto print(int) -> void;
    auto generate() -> void;
};

struct Print : Statement {
    bool isPrintLine;
    vector<Expression*> arguments;
    auto print(int) -> void;
    auto generate() -> void;
};

struct ExpressionStatement : Statement {
    Expression* expression;
    auto print(int) -> void;
    auto generate() -> void;
};

struct Or : Expression {
    Expression* lhs;
    Expression* rhs;
    auto print(int) -> void;
    auto generate() -> void;
};

struct And : Expression {
    Expression* lhs;
    Expression* rhs;
    auto print(int) -> void;
    auto generate() -> void;
};

struct Relational : Expression {
    Kind kind;
    Expression* lhs;
    Expression* rhs;
    auto print(int) -> void;
    auto generate() -> void;
};

struct Arithmetic : Expression {
    Kind kind;
    Expression* lhs;
    Expression* rhs;
    auto print(int) -> void;
    auto generate() -> void;
};

struct Unary : Expression {
    Kind kind;
    Expression* sub;
    auto print(int) -> void;
    auto generate() -> void;
};

struct Call : Expression {
    Expression* sub;
    vector<Expression*> parameters;
    auto print(int) -> void;
    auto generate() -> void;
};

// 배열, map 원소의 참조
// arr[0]
struct GetElement : Expression {
    Expression* sub;
    Expression* index;
    auto print(int) -> void;
    auto generate() -> void;
};

// arr[0] = 3;
// 할당은 statement 아닌가?
struct SetElement : Expression {
    Expression* sub;
    Expression* index;
    Expression* value;
    auto print(int) -> void;
    auto generate() -> void;
};

// 변수 참조
struct GetVariable : Expression {
    wstring name;
    auto print(int) -> void;
    auto generate() -> void;
};

// 변수 수정
struct SetVariable : Expression {
    wstring name;
    Expression* value;
    auto print(int) -> void;
    auto generate() -> void;
};

struct NullLiteral : Expression {
    auto print(int) -> void;
    auto generate() -> void;
};
struct BooleanLiteral : Expression {
    bool value = false;
    auto print(int) -> void;
    auto generate() -> void;
};
struct NumberLiteral : Expression {
    double value = 0.0;
    auto print(int) -> void;
    auto generate() -> void;
};
struct StringLiteral : Expression {
    wstring value;
    auto print(int) -> void;
    auto generate() -> void;
};
struct ArrayLiteral : Expression {
    vector<Expression*> values;
    auto print(int) -> void;
    auto generate() -> void;
};
struct MapLiteral : Expression {
    map<wstring, Expression*> values;
    auto print(int) -> void;
    auto generate() -> void;
};
#endif //KORLANG_NODE_H