//
// Created by yoon on 3/15/25.
//

#include "Kind.h"

static map<wstring, Kind> stringToKind = {
        {L"#unknown", Kind::Unknown},
        {L"#끝",       Kind::EndOfLine},

        {L"빈값",       Kind::NullLiteral},
        {L"참",        Kind::TrueLiteral},
        {L"거짓",       Kind::FalseLiteral},
        {L"#숫자-리터럴",  Kind::NumberLiteral},
        {L"#문자열-리터럴", Kind::StringLiteral},
        {L"#식별자",     Kind::Identifier},

        {L"함수",       Kind::Function},
        {L"반환",       Kind::Return},
        {L"변수",       Kind::Variable},
        {L"반복",       Kind::For},
        {L"끊기",       Kind::Break},
        {L"계속하기",     Kind::Continue},
        {L"만약",       Kind::If},
        {L"그게아니라",    Kind::Elif},
        {L"아니면",      Kind::Else},
        {L"출력",       Kind::Print},
        {L"출력줄바꿈",  Kind::PrintLine},

        {L"그리고",      Kind::LogicalAnd},
        {L"또는",       Kind::LogicalOr},

        {L"=",        Kind::Assignment},

        {L"+",        Kind::Add},
        {L"-",        Kind::Subtract},
        {L"*",        Kind::Multiply},
        {L"/",        Kind::Divide},
        {L"%",        Kind::Modulo},

        {L"==",       Kind::Equal},
        {L"!=",       Kind::NotEqual},
        {L"<",        Kind::LessThan},
        {L">",        Kind::GreaterThan},
        {L"<=",       Kind::LessOrEqual},
        {L">=",       Kind::GreaterOrEqual},

        {L",",        Kind::Comma},
        {L":",        Kind::Colon},
        {L";",        Kind::Semicolon},
        {L"(",        Kind::LeftParen},
        {L")",        Kind::RightParen},
        {L"{",        Kind::LeftBrace},
        {L"}",        Kind::RightBrace},
        {L"[",        Kind::LeftBraket},
        {L"]",        Kind::RightBraket},
};

auto toKind(const wstring& string) -> Kind {
    if (stringToKind.count(string))
    {
        return stringToKind.at(string);
    }
    return Kind::Unknown;
}

static auto kindToString = [] {
    map<Kind, wstring> result;
    for (auto& [key, value] : stringToKind)
        result[value] = key;
    return result;
}();

auto toString(Kind type)->wstring {
    if (kindToString.count(type))
        return kindToString.at(type);
    return L"";
}
