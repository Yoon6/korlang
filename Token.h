//
// Created by yoon on 3/15/25.
//

#ifndef KORLANG_TOKEN_H
#define KORLANG_TOKEN_H
#include "Kind.h"
#include <locale>
#include <codecvt>

struct Token {
    Kind kind = Kind::Unknown;
    wstring string;
};

auto operator<<(ostream&, Token&)->ostream&;


#endif //KORLANG_TOKEN_H
