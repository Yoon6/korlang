#include <iostream>
#include <vector>
#include <iomanip>

#include "Token.h"
#include "Node.h"

vector<Token> scan(string sourceCode);
Program* parse(vector<Token> tokenList);
auto printTokenList(vector<Token>) -> void;
auto printSyntaxTree(Program*) -> void;

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
	Program* syntaxTree = parse(tokenList);

    printSyntaxTree(syntaxTree);
    //printTokenList(tokenList);

    return 0;
}

auto printTokenList(vector<Token> tokenList) -> void {
    cout << setw(12) << left << "KIND" << "STRING" << endl;
    cout << string(23, '-') << endl;
    for (auto &token: tokenList)
        cout << token << endl;
}

auto operator<<(ostream &stream, Token &token) -> ostream & {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return stream << setw(12) << left << converter.to_bytes(toString(token.kind)) << setw(12) << right << '\t' << converter.to_bytes(token.string);
}