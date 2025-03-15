//
// Created by yoon on 3/15/25.
//

#include "Kind.h"
#include <map>

static map<string, Kind> stringToToken = {
        {"#unknown",    Kind::Unknown},
        {"#끝", Kind::EndOfLine},

        {"빈값",        Kind::NullLiteral},
        {"참",        Kind::TrueLiteral},
        {"거짓",       Kind::FalseLiteral},
        {"#숫자-리터럴",     Kind::NumberLiteral},
        {"#문자열-리터럴",     Kind::StringLiteral},
        {"#식별자",    Kind::Identifier},

        {"함수",    Kind::Function},
        {"반환",      Kind::Return},
        {"변수",         Kind::Variable},
        {"반복",         Kind::For},
        {"끊기",       Kind::Break},
        {"계속하기",    Kind::Continue},
        {"만약",          Kind::If},
        {"그게아니라",        Kind::Elif},
        {"아니면",        Kind::Else},
        {"출력",       Kind::Print},

        {"그리고",         Kind::LogicalAnd},
        {"또는",          Kind::LogicalOr},

        {"=",           Kind::Assignment},

        {"+",           Kind::Add},
        {"-",           Kind::Subtract},
        {"*",           Kind::Multiply},
        {"/",           Kind::Divide},
        {"%",           Kind::Modulo},

        {"==",          Kind::Equal},
        {"!=",          Kind::NotEqual},
        {"<",           Kind::LessThan},
        {">",           Kind::GreaterThan},
        {"<=",          Kind::LessOrEqual},
        {">=",          Kind::GreaterOrEqual},

        {",",           Kind::Comma},
        {":",           Kind::Colon},
        {";",           Kind::Semicolon},
        {"(",           Kind::LeftParen},
        {")",           Kind::RightParen},
        {"{",           Kind::LeftBrace},
        {"}",           Kind::RightBrace},
        {"[",           Kind::LeftBraket},
        {"]",           Kind::RightBraket},
};