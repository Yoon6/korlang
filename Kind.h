//
// Created by yoon on 3/15/25.
//

#ifndef KORLANG_KIND_H
#define KORLANG_KIND_H

#include <string>
#include <iostream>
#include <map>

using namespace std;

enum class Kind {
    Unknown, EndOfLine,

    NullLiteral,
    TrueLiteral, FalseLiteral,
    NumberLiteral, StringLiteral,
    Identifier,

    Function, Return,
    Variable,
    For, Break, Continue,
    If, Elif, Else,
    Print,

    LogicalAnd, LogicalOr,
    Assignment,
    Add, Subtract,
    Multiply, Divide, Modulo,
    Equal, NotEqual,
    LessThan, GreaterThan,
    LessOrEqual, GreaterOrEqual,

    Comma, Colon, Semicolon,
    LeftParen, RightParen,
    LeftBrace, RightBrace,
    LeftBraket, RightBraket,
};

auto toKind(const wstring&) -> Kind;
auto toString(Kind)->wstring;

#endif //KORLANG_KIND_H
