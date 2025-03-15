//
// Created by yoon on 3/15/25.
//

#include <vector>
#include "Token.h"


static string::iterator current;

vector<Token> scan(string sourceCode)
{
    vector<Token> result;
    sourceCode += '\0'; // Add null character
    current = sourceCode.begin();

    while (*current != '\0')
    {


    }

    result.push_back({Kind::EndOfLine});
    return result;
}