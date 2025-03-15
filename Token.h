//
// Created by yoon on 3/15/25.
//

#ifndef KORLANG_TOKEN_H
#define KORLANG_TOKEN_H
#include "Kind.h"

struct Token {
    Kind kind = Kind::Unknown;
    string string;
};

#endif //KORLANG_TOKEN_H
