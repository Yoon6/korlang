#include <iostream>
#include <vector>
#include <iomanip>
#include <tuple>

#include "Token.h"
#include "Node.h"
#include "Code.h"

vector<Token> scan(string sourceCode);
Program* parse(vector<Token> tokenList);
tuple<vector<Code>, map<wstring, size_t>> generate(Program* syntaxTree);
void execute(vector<Code> codeList, map<wstring, size_t> functionTables);

auto printTokenList(vector<Token>) -> void;
// auto printSyntaxTree(Program*) -> void;
// auto printObjectCode(tuple<vector<Code>, map<wstring, size_t>>) -> void;
extern "C" void run(string sourceCode)
{
    printf("Run\n");
    printf("Scan\n");
    vector<Token> tokenList = scan(sourceCode);
    printTokenList(tokenList);
    printf("Parse\n");
	Program* syntaxTree = parse(tokenList);
    printf("Generate\n");
	auto objectCode = generate(syntaxTree);
    printf("Execute\n");
    execute(get<0>(objectCode), get<1>(objectCode));
}


int main() {
//     string sourceCode = R"(
//         함수 시작() {
//             출력줄바꿈(11 + 2);
//             출력줄바꿈("Hello");
//             출력줄바꿈(팩토리얼(3));
//         }

//         함수 팩토리얼(숫자) {
//             만약 (숫자 < 2) {
//                 반환 1;
//             }
//             반환 숫자 * 팩토리얼(숫자 - 1);
//         }
//     )";

//     run(sourceCode);

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