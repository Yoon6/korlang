#include <iostream>
#include "Token.h"
#include <vector>

vector<Token> scan(string sourceCode);

int main() {
    string sourceCode = R"(
        함수 시작() {
            출력("안녕");
            출력("\n");
            출력(1 + 2 * 3);
            출력("\n");
        }
    )";

    vector<Token> tokenList = scan(sourceCode);
//    printTokenList(tokenList);

    return 0;
}
