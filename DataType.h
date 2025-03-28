//
// Created by yoon on 3/28/25.
//

#ifndef KORLANG_DATATYPE_H
#define KORLANG_DATATYPE_H

#include <any>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <functional>
#include "Node.h"

struct Object {
    bool isMarked = false;
    virtual ~Object() {};
};

struct Array: Object {
    vector<any> values;
};

struct Map: Object {
    map<wstring, any> values;
};

auto isMap(any value) -> bool;
auto toMap(any value) -> Map*;
auto getValueOfMap(any object, any index) -> any;
auto setValueOfMap(any object, any index, any value) -> any;

auto isArray(any value) -> bool;
auto toArray(any value) -> Array*;
auto getValueOfArray(any object, any index) -> any;
auto setValueOfArray(any object, any index, any value) -> any;

auto isNull(any value) -> bool;

auto isBuiltinFunction(any value) -> bool;
auto toBuiltinFunction(any value) -> function<any(vector<any>)>;

auto isBoolean(any value) -> bool;
auto toBoolean(any value) -> bool;
auto isTrue(any value) -> bool;
auto isFalse(any value) -> bool;

auto isString(any value) -> bool;
auto toString(any value) -> wstring;

auto isNumber(any value) -> bool;
auto toNumber(any value) -> double;

auto isSize(any value) -> bool;
auto toSize(any value) -> size_t;

auto operator<<(ostream& stream, any& value) -> ostream&;

#endif //KORLANG_DATATYPE_H
