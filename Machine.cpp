//
// Created by yoon on 3/28/25.
//

#include <vector>
#include <map>
#include <any>
#include <list>

#include "Code.h"
#include "DataType.h"

using namespace std;

struct StackFrame {
    vector<any> variables;
    vector<any> operandStack;
    size_t instructionPointer = 0;
};

static list<Object *> objects;
static vector<StackFrame> callStack;
static map<wstring, any> global;
extern map<wstring, function<any(vector<any>)>> builtinFunctionTable;

auto markObject(any value) -> void {
    if (isArray(value)) {
        if (toArray(value)->isMarked) return;
        toArray(value)->isMarked = true;
        for (auto &value: toArray(value)->values) {
            markObject(value);
        }
    } else if (isMap(value)) {
        if (toMap(value)->isMarked) return;
        toMap(value)->isMarked = true;
        for (auto &[key, value]: toMap(value)->values) {
            markObject(value);
        }
    }
}

auto sweepObject() -> void {
    objects.remove_if([](Object* object) {
        if (object->isMarked) {
            object->isMarked = false;
            return false;
        }

        delete object;
        return true;
    });
}

auto collectGarage() -> void {
    for (auto &stackFrame: callStack) {
        for (auto &value: stackFrame.operandStack) {
            markObject(value);
        }
        for (auto &value: stackFrame.variables) {
            markObject(value);
        }
    }
    for (auto &[key, value]: global) {
        markObject(value);
    }
    sweepObject();
}

void pushOperand(any value) {
    callStack.back().operandStack.push_back(value);
}

any popOperand() {
    auto value = callStack.back().operandStack.back();
    callStack.back().operandStack.pop_back();
    return value;
}

any peekOperand() {
    auto value = callStack.back().operandStack.back();
    return value;
}

void execute(vector<Code> codeList, map<wstring, size_t> functionTables) {
    callStack.emplace_back();
    while (true) {
        auto code = codeList[callStack.back().instructionPointer];
        switch (code.instruction) {
            case Instruction::Exit:
                callStack.pop_back();
                return;
            case Instruction::Call: {
                auto operand = popOperand();
                if (isSize(operand)) {
                    StackFrame stackFrame;
                    stackFrame.instructionPointer = toSize(operand);
                    for (auto i = 0; i < toSize(code.operand); i++) {
                        stackFrame.variables.push_back(callStack.back().operandStack.back());
                        callStack.back().operandStack.pop_back();
                    }
                    callStack.push_back(stackFrame);
                    continue;
                }
                if (isBuiltinFunction(operand)) {
                    vector<any> arguments;
                    for (auto i = 0; i < toSize(code.operand); i++) {
                        arguments.push_back(popOperand());
                    }
                    pushOperand(toBuiltinFunction(operand)(arguments));
                    break;
                }

                pushOperand(nullptr);
                break;
            }
            case Instruction::Alloca: {
                auto extraSize = toSize(code.operand);
                auto currentSize = callStack.back().variables.size();
                callStack.back().variables.resize(extraSize + currentSize);
                break;
            }
            case Instruction::Return: {
                any result = nullptr;
                if (!callStack.back().operandStack.empty()) {
                    result = callStack.back().operandStack.back();
                }
                callStack.pop_back();
                callStack.back().operandStack.push_back(result);
                collectGarage();
                break;
            }
            case Instruction::Jump: {
                callStack.back().instructionPointer = toSize(code.operand);
                continue;
            }
            case Instruction::ConditionJump: {
                auto condition = popOperand();
                if (isTrue(condition)) {
                    break;
                }

                callStack.back().instructionPointer = toSize(code.operand);
                continue;
            }
            case Instruction::Print: {
                for (auto i = 0; i < toSize(code.operand); i++) {
                    auto value = popOperand();
                    cout << value;
                }
                break;
            }
            case Instruction::PrintLine: {
                cout << '\n';
                break;
            }
            case Instruction::LogicalOr: {
                auto value = popOperand();
                if (isTrue(value)) {
                    pushOperand(value);
                    callStack.back().instructionPointer = toSize(code.operand);
                    continue;
                }
                break;
            }
            case Instruction::LogicalAnd: {
                auto value = popOperand();
                if (isFalse(value)) {
                    pushOperand(value);
                    callStack.back().instructionPointer = toSize(code.operand);
                    continue;
                }
                break;
            }
            case Instruction::Add: {
                auto rValue = popOperand();
                auto lValue = popOperand();
                if (isNumber(rValue) && isNumber(lValue)) {
                    pushOperand(toNumber(rValue) + toNumber(lValue));
                } else if (isString(rValue) && isString(lValue)) {
                    pushOperand(toString(rValue) + toString(lValue));
                } else {
                    pushOperand(0.0);
                }

                break;
            }
            case Instruction::Subtract: {
                auto rValue = popOperand();
                auto lValue = popOperand();
                if (isNumber(rValue) && isNumber(lValue)) {
                    pushOperand(toNumber(lValue) - toNumber(rValue));
                } else {
                    pushOperand(0.0);
                }
                break;
            }
            case Instruction::Multiply: {
                auto rValue = popOperand();
                auto lValue = popOperand();
                if (isNumber(rValue) && isNumber(lValue)) {
                    pushOperand(toNumber(lValue) * toNumber(rValue));
                } else {
                    pushOperand(0.0);
                }
                break;
            }
            case Instruction::Divide: {
                auto rValue = popOperand();
                auto lValue = popOperand();
                if (isNumber(rValue) && isNumber(lValue) && toNumber(rValue) != 0) {
                    pushOperand(toNumber(lValue) / toNumber(rValue));
                } else {
                    pushOperand(0.0);
                }
                break;
            }
            case Instruction::Modulo: {
                auto rValue = popOperand();
                auto lValue = popOperand();
                if (isNumber(rValue) && isNumber(lValue) && toNumber(rValue) != 0) {
                    pushOperand(fmod(toNumber(lValue), toNumber(rValue)));
                } else {
                    pushOperand(0.0);
                }
                break;
            }
            case Instruction::Equal: {
                auto rValue = popOperand();
                auto lValue = popOperand();
                if (isNumber(rValue) && isNumber(lValue)) {
                    pushOperand(toNumber(lValue) == toNumber(rValue));
                } else if (isString(rValue) && isString(lValue)) {
                    pushOperand(toString(lValue) == toString(rValue));
                } else if (isBoolean(rValue) && isBoolean(lValue)) {
                    pushOperand(toBoolean(lValue) == toBoolean(rValue));
                } else if (isNull(rValue) && isNull(lValue)) {
                    pushOperand(true);
                } else {
                    pushOperand(false);
                }
                break;
            }
            case Instruction::NotEqual: {
                auto rValue = popOperand();
                auto lValue = popOperand();
                if (isNumber(rValue) && isNumber(lValue)) {
                    pushOperand(toNumber(lValue) != toNumber(rValue));
                } else if (isString(rValue) && isString(lValue)) {
                    pushOperand(toString(lValue) != toString(rValue));
                } else if (isBoolean(rValue) && isBoolean(lValue)) {
                    pushOperand(toBoolean(lValue) != toBoolean(rValue));
                } else if (isNull(rValue) && isNull(lValue)) {
                    pushOperand(false);
                } else if (isNull(rValue) || isNull(lValue)) {
                    pushOperand(true);
                } else {
                    pushOperand(false);
                }
                break;
            }
            case Instruction::LessThan: {
                auto rValue = popOperand();
                auto lValue = popOperand();
                if (isNumber(rValue) && isNumber(lValue)) {
                    pushOperand(toNumber(lValue) < toNumber(rValue));
                } else {
                    pushOperand(false);
                }
                break;
            }
            case Instruction::GreaterThan: {
                auto rValue = popOperand();
                auto lValue = popOperand();
                if (isNumber(rValue) && isNumber(lValue)) {
                    pushOperand(toNumber(lValue) > toNumber(rValue));
                } else {
                    pushOperand(false);
                }
                break;
            }
            case Instruction::LessOrEqual: {
                auto rValue = popOperand();
                auto lValue = popOperand();
                if (isNumber(rValue) && isNumber(lValue)) {
                    pushOperand(toNumber(lValue) <= toNumber(rValue));
                } else {
                    pushOperand(false);
                }
                break;
            }
            case Instruction::GreaterOrEqual: {
                auto rValue = popOperand();
                auto lValue = popOperand();
                if (isNumber(rValue) && isNumber(lValue)) {
                    pushOperand(toNumber(lValue) >= toNumber(rValue));
                } else {
                    pushOperand(false);
                }

                break;
            }
            case Instruction::Absolute: {
                auto value = popOperand();
                if (isNumber(value)) {
                    pushOperand(abs(toNumber(value)));
                } else {
                    pushOperand(0.0);
                }
                break;
            }
            case Instruction::ReverseSign: {
                auto value = popOperand();
                if (isNumber(value)) {
                    pushOperand(toNumber(value) * -1);
                } else {
                    pushOperand(0.0);
                }
                break;
            }
            case Instruction::GetElement: {
                auto index = popOperand();
                auto object = popOperand();
                if (isArray(object) && isNumber(index)) {
                    pushOperand(getValueOfArray(object, index));
                } else if (isMap(object) && isString(index)) {
                    pushOperand(getValueOfMap(object, index));
                } else {
                    pushOperand(nullptr);
                }
                break;
            }
            case Instruction::SetElement: {
                auto index = popOperand();
                auto object = popOperand();

                if (isArray(object) && isNumber(index)) {
                    setValueOfArray(object, index, peekOperand());
                } else if (isMap(object) && isString(index)) {
                    setValueOfMap(object, index, peekOperand());
                }

                break;
            }
            case Instruction::GetGlobal: {
                auto name = toString(code.operand);
                if (functionTables.count(name)) {
                    pushOperand(functionTables[name]);
                } else if (builtinFunctionTable.count(name)) {
                    pushOperand(builtinFunctionTable[name]);
                } else if (global.count(name)) {
                    pushOperand(global[name]);
                } else {
                    pushOperand(nullptr);
                }
                break;
            }
            case Instruction::SetGlobal: {
                auto name = toString(code.operand);
                global[name] = peekOperand();
                break;
            }
            case Instruction::GetLocal: {
                auto index = toSize(code.operand);
                pushOperand(callStack.back().variables[index]);
                break;
                break;
            }
            case Instruction::SetLocal: {
                auto index = toSize(code.operand);
                callStack.back().variables[index] = peekOperand();

                break;
            }
            case Instruction::PushNull: {
                pushOperand(nullptr);
                break;
            }
            case Instruction::PushBoolean: {
                pushOperand(code.operand);
                break;
            }
            case Instruction::PushNumber: {
                pushOperand(code.operand);
                break;
            }
            case Instruction::PushString: {
                pushOperand(code.operand);
                break;
            }
            case Instruction::PushArray: {
                auto result = new Array();
                auto size = toSize(code.operand);
                for (auto i = size; i > 0; i--) {
                    result->values.push_back(popOperand());
                }
                pushOperand(result);
                objects.push_back(result);
                break;
            }
            case Instruction::PushMap: {
                auto result = new Map();
                auto size = toSize(code.operand);
                for (auto i = size; i > 0; i--) {
                    auto value = popOperand();
                    auto key = toString(popOperand());
                    result->values[key] = value;
                }
                pushOperand(result);
                objects.push_back(result);
                break;
            }
            case Instruction::PopOperand: {
                popOperand();
                break;
            }
        }
        callStack.back().instructionPointer += 1;
    }
}
