//
// Created by yoon on 3/28/25.
//
#include <codecvt>
#include "DataType.h"

auto isBuiltinFunction(any value) -> bool {
    return value.type() == typeid(function<any(vector<any>)>);
}

auto toBuiltinFunction(any value) -> function<any(vector<any>)> {
    return any_cast<function<any(vector<any>)>>(value);
}

auto isArray(any value) -> bool {
    return value.type() == typeid(Array*);
}

auto toArray(any value) -> Array* {
    return any_cast<Array*>(value);
}

auto getValueOfArray(any object, any index) -> any {
    auto i = static_cast<size_t>(toNumber(index));
    if (i >= 0 && i < toArray(object)->values.size())
    {
        return toArray(object)->values[i];
    }
    return nullptr;
}

auto setValueOfArray(any object, any index, any value) -> any {
    auto i = static_cast<size_t>(toNumber(index));
    if (i >= 0 && i < toArray(object)->values.size())
    {
        toArray(object)->values[i] = value;
    }
    return value;
}

auto isMap(any value) -> bool {
    return value.type() == typeid(Map*);
}

auto toMap(any value) -> Map* {
    return any_cast<Map*>(value);
}

auto getValueOfMap(any object, any index) -> any {
    if (toMap(object)->values.count(toString(index)))
    {
        return toMap(object)->values[toString(index)];
    }
    return nullptr;
}

auto setValueOfMap(any object, any index, any value) -> any {
    toMap(object)->values[toString(index)] = value;
    return value;
}

auto isNull(any value) -> bool {
    return value.type() == typeid(nullptr_t);
}

auto isBoolean(any value) -> bool {
    return value.type() == typeid(bool);
}

auto toBoolean(any value) -> bool {
    return any_cast<bool>(value);
}

auto isTrue(any value) -> bool {
    return isBoolean(value) && toBoolean(value);
}

auto isFalse(any value) -> bool {
    return isBoolean(value) && !toBoolean(value);
}

auto isNumber(any value) -> bool {
    return value.type() == typeid(double);
}

auto toNumber(any value) -> double {
    return any_cast<double>(value);
}

auto isString(any value) -> bool {
    return value.type() == typeid(wstring);
}

auto toString(any value) -> wstring {
    return any_cast<wstring>(value);
}

auto isSize(any value) -> bool {
    return value.type() == typeid(size_t);
}

auto toSize(any value) -> size_t {
    return any_cast<size_t>(value);
}

auto operator<<(ostream& stream, any& value)->ostream& {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    if (isNull(value))
    {
        stream << "빈값";
    }
    else if (isBoolean(value))
    {
        string strBool = any_cast<bool>(value) ? "참" : "거짓";
        stream << strBool;
    }
    else if (isNumber(value))
    {
        printf("%g", toNumber(value));
    }
    else if (isString(value))
    {
        stream << converter.to_bytes(toString(value));
    }
    else if (isArray(value))
    {
        stream << "[";
        for (auto& value : toArray(value)->values)
        {
            stream << value << " ";
        }
        stream << "]";
    }
    else if (isMap(value))
    {
        stream << "{";
        for (auto& [key, value] : toMap(value)->values)
        {
            stream << converter.to_bytes(key) << ":" << value << " ";
        }
        stream << "}";
    }
    return stream;
}