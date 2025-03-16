//
// Created by yoon on 3/15/25.
//

#include <vector>
#include "Token.h"

static wstring::iterator current;

enum class CharType {
    Unknown,
    WhiteSpace,
    NumberLiteral,
    StringLiteral,
    IdentifierAndKeyword,
    OperatorAndPunctuator
};

bool isHangul(wchar_t wc) {
    return (wc >= 0xAC00 && wc <= 0xD7A3);
}

CharType getCharType(wchar_t c) {
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
        return CharType::WhiteSpace;
    }
    if ('0' <= c && c <= '9') {
        return CharType::NumberLiteral;
    }
    if (c == '\"') {
        return CharType::StringLiteral;
    }
    if ('a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || isHangul(c)) {
        return CharType::IdentifierAndKeyword;
    }
    if ('!' <= c && c <= '/' ||
        ':' <= c && c <= '@' ||
        '[' <= c && c <= '`' ||
        '{' <= c && c <= '~') {
        return CharType::OperatorAndPunctuator;
    }
    return CharType::Unknown;
}

bool isCharType(wchar_t wc, CharType type) {
    switch (type) {
        case CharType::NumberLiteral:
            return '0' <= wc && wc <= '9';
        case CharType::StringLiteral:
            return (33 <= wc && wc <= 126 && wc != '\"') || isHangul(wc);
        case CharType::IdentifierAndKeyword:
            return '0' <= wc && wc <= '9' || 'a' <= wc && wc <= 'z' || 'A' <= wc && wc <= 'Z' || isHangul(wc);
        case CharType::OperatorAndPunctuator:
            return '!' <= wc && wc <= '/' || ':' <= wc && wc <= '@' || '[' <= wc && wc <= '`' || '{' <= wc && wc <= '~';
        default:
            return false;
    }
}

Token scanNumberLiteral() {
    wstring string;
    while (isCharType(*current, CharType::NumberLiteral)) {
        string += *(current++);
    }
    if (*current == '.') {
        string += *(current++);
        while (isCharType(*current, CharType::NumberLiteral)) {
            string += *(current++);
        }
    }
    return Token{Kind::NumberLiteral, string};
}

Token scanStringLiteral() {
    wstring string;
    current++;
    while (isCharType(*current, CharType::StringLiteral)) {
        string += *(current++);
    }
    if (*current != '\"') {
        cout << "문자열 종료 문자가 없습니다.";
        exit(1);
    }
    current++;
    return Token{Kind::StringLiteral, string};
}

Token scanIdentifierAndKeyword() {
    wstring string;
    while (isCharType(*current, CharType::IdentifierAndKeyword)) {
        string += *(current++);
    }
    auto kind = toKind(string);
    if (kind == Kind::Unknown) {
        kind = Kind::Identifier;
    }
    return Token{kind, string};
}

Token scanOperatorAndPunctuator() {
    wstring string;
    while (isCharType(*current, CharType::OperatorAndPunctuator))
    {
        string += *(current++);
    }
    while (!string.empty() && toKind(string) == Kind::Unknown)
    {
        string.pop_back();
        current--;
    }
    if (string.empty())
    {
        cout << *current << " 사용할 수 없는 문자입니다.";
        exit(1);
    }
    return Token{toKind(string), string};

}

vector<Token> scan(string sourceCode) {
    vector<Token> result;
    sourceCode += '\0'; // Add null character
    wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wSourceCode = converter.from_bytes(sourceCode);

    current = wSourceCode.begin();

    while (*current != '\0') {
        switch (getCharType(*current)) {
            case CharType::WhiteSpace:
                current += 1;
                break;
            case CharType::NumberLiteral:
                result.push_back(scanNumberLiteral());
                break;
            case CharType::StringLiteral:
                result.push_back(scanStringLiteral());
                break;
            case CharType::IdentifierAndKeyword:
                result.push_back(scanIdentifierAndKeyword());
                break;
            case CharType::OperatorAndPunctuator:
                result.push_back(scanOperatorAndPunctuator());
                break;
            default:
                cout << *current << " 사용할 수 없는 문자입니다.";
                exit(1);
        }
    }

    result.push_back({Kind::EndOfLine});
    return result;
}