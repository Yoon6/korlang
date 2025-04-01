//
// Created by yoon on 3/29/25.
//

#include "DataType.h"

using namespace std;

map<wstring, function<any(vector<any>)>> builtinFunctionTable = {
        {L"길이", [](vector<any> values) -> any {
            if (values.size() == 1 && isArray(values[0])) {
                return static_cast<size_t>(toArray(values[0])->values.size());
            }
            if (values.size() == 1 && isMap(values[0])) {
                return static_cast<size_t>(toMap(values[0])->values.size());
            }
            return static_cast<size_t>(0);
        }},
        {L"삽입",   [](vector<any> values) -> any {
            if (values.size() == 2 && isArray(values[0])) {
                toArray(values[0])->values.push_back(values[1]);
                return values[0];
            }
            return nullptr;

        }},
        {L"꺼내기",    [](vector<any> values) -> any {
            if (values.size() == 1 && isArray(values[0]) && toArray(values[0])->values.size() > 0) {
                auto result = toArray(values[0])->values.back();
                toArray(values[0])->values.pop_back();
                return result;
            }
            return nullptr;
        }},
        {L"지우기",  [](vector<any> values) -> any {
            if (values.size() == 2 && isMap(values[0]) && isString(values[1]) && toMap(values[0])->values.count(toString(values[1])))
            {
                auto result = toMap(values[0])->values.at(toString(values[1]));
                toMap(values[0])->values.erase(toString(values[1]));
                return result;
            }
            return nullptr;
        }},
        {L"시각",  [](vector<any> values) -> any {
            return static_cast<double>(clock());

        }},
        {L"제곱근",   [](vector<any> values) -> any {
            return sqrt(toNumber(values[0]));
        }},
};